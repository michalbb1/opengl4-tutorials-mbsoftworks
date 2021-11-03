// STL
#include <algorithm>
#include <mutex>
#include <cmath>

// GLM
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

// Project
#include "freeTypeFont.h"
#include "shaderManager.h"
#include "shaderProgramManager.h"
#include "samplerManager.h"
#include "matrixManager.h"
#include "textureManager.h"

using namespace std;

const int FreeTypeFont::CHARACTERS_TEXTURE_SIZE = 512;
const std::string FreeTypeFont::FREETYPE_FONT_PROGRAM_KEY = "freetype_font";
const std::string FreeTypeFont::FREETYPE_FONT_SAMPLER_KEY = "freetype_font";

FreeTypeFont::FreeTypeFont()
{
    static std::once_flag prepareOnceFlag;
    std::call_once(prepareOnceFlag, []()
    {
        auto& sm = ShaderManager::getInstance();
        sm.loadVertexShader(FREETYPE_FONT_PROGRAM_KEY, "data/shaders/tut012/font2D.vert");
        sm.loadFragmentShader(FREETYPE_FONT_PROGRAM_KEY, "data/shaders/tut012/font2D.frag");

        auto& spm = ShaderProgramManager::getInstance();
        auto& shaderProgram = spm.createShaderProgram(FREETYPE_FONT_PROGRAM_KEY);
        shaderProgram.addShaderToProgram(sm.getVertexShader(FREETYPE_FONT_PROGRAM_KEY));
        shaderProgram.addShaderToProgram(sm.getFragmentShader(FREETYPE_FONT_PROGRAM_KEY));
        shaderProgram.linkProgram();

        auto& smm = SamplerManager::getInstance();
        auto& sampler = smm.createSampler(FREETYPE_FONT_SAMPLER_KEY, MAG_FILTER_BILINEAR, MIN_FILTER_NEAREST);
        sampler.setRepeat(false);
    });

    addCharacterRange(32, 127); // Add ASCII characters always
}

FreeTypeFont::~FreeTypeFont()
{
    deleteFont();
}

void FreeTypeFont::addCharacterRange(unsigned int characterFrom, unsigned int characterTo)
{
    _characterRanges.push_back(CharacterRange(characterFrom, characterTo));
}

bool FreeTypeFont::loadFont(const std::string& fontFilePath, int pixelSize)
{
    if (pixelSize < 1 || pixelSize > CHARACTERS_TEXTURE_SIZE)
    {
        throw std::runtime_error(string_utils::formatString("Cannot load font, because of invalid texture size (must be between 1 and {})", CHARACTERS_TEXTURE_SIZE));
    }

    FT_Library freeTypeLibrary;
    FT_Face freeTypeFace;

    auto ftError = FT_Init_FreeType(&freeTypeLibrary);
    if (ftError) {
        return false;
    }

    ftError = FT_New_Face(freeTypeLibrary, fontFilePath.c_str(), 0, &freeTypeFace);
    if (ftError) {
        return false;
    }

    FT_Set_Pixel_Sizes(freeTypeFace, 0, pixelSize);
    _pixelSize = pixelSize;

    std::vector<unsigned char> textureData(CHARACTERS_TEXTURE_SIZE*CHARACTERS_TEXTURE_SIZE, 0);
    auto currentPixelPositionRow = 0;
    auto currentPixelPositionCol = 0;
    auto rowHeight = 0;
    auto currentRenderIndex = 0;
    std::unique_ptr<Texture> texture = std::make_unique<Texture>();

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    _vbo.createVBO();
    _vbo.bindVBO();

    auto finalizeTexture = [this, &texture, &textureData](bool createNext)
    {
        texture->createFromData(textureData.data(), CHARACTERS_TEXTURE_SIZE, CHARACTERS_TEXTURE_SIZE, GL_DEPTH_COMPONENT, true);
        _textures.push_back(std::move(texture));
        if (createNext)
        {
            texture = std::make_unique<Texture>();
            memset(textureData.data(), 0, textureData.size());
        }
    };
    
    for (const auto& characterRange : _characterRanges)
    {
        for (auto c = characterRange.characterCodeFrom; c <= characterRange.characterCodeTo;)
        {
            FT_Load_Glyph(freeTypeFace, FT_Get_Char_Index(freeTypeFace, c), FT_LOAD_DEFAULT);
            FT_Render_Glyph(freeTypeFace->glyph, FT_RENDER_MODE_NORMAL);

            const auto* ptrBitmap = &freeTypeFace->glyph->bitmap;
            const int bmpWidth = ptrBitmap->width;
            const int bmpHeight = ptrBitmap->rows;
            const auto rowsLeft = CHARACTERS_TEXTURE_SIZE - currentPixelPositionRow;
            const auto colsLeft = CHARACTERS_TEXTURE_SIZE - currentPixelPositionCol;

            rowHeight = std::max(rowHeight, static_cast<int>(bmpHeight));

            if (colsLeft < bmpWidth)
            {
                currentPixelPositionCol = 0;
                currentPixelPositionRow += rowHeight + 1;
                rowHeight = 0;
                continue;
            }

            if (rowsLeft < bmpHeight)
            {
                finalizeTexture(true);
                currentPixelPositionCol = 0;
                currentPixelPositionRow = 0;
                rowHeight = 0;
                continue;
            }
            
            auto& charProps = _characterProperties[c]; // This also creates entry, if it does not exist
            charProps.characterCode = c;
            charProps.width = freeTypeFace->glyph->metrics.width >> 6;
            charProps.bearingX = freeTypeFace->glyph->metrics.horiBearingX >> 6;
            charProps.advanceX = freeTypeFace->glyph->metrics.horiAdvance >> 6;
            charProps.height = freeTypeFace->glyph->metrics.height >> 6;
            charProps.bearingY = freeTypeFace->glyph->metrics.horiBearingY >> 6;

            // If character is not renderable, e.g. space, then don't prepare rendering data for it
            if (bmpWidth == 0 && bmpHeight == 0) {
                charProps.renderIndex = -1;
                charProps.textureIndex = -1;
                c++;
                continue;
            }

            for (auto i = 0; i < bmpHeight; i++)
            {
                int globalRow = currentPixelPositionRow + i;
                int reversedRow = bmpHeight - i - 1;
                memcpy(textureData.data() + globalRow * CHARACTERS_TEXTURE_SIZE + currentPixelPositionCol, ptrBitmap->buffer + reversedRow * bmpWidth, bmpWidth);
            }

            // Setup vertices according to FreeType glyph metrics
            // You can find it here: https://www.freetype.org/freetype2/docs/glyphs/glyphs-3.html
            glm::vec2 vertices[] =
            {
                glm::vec2(static_cast<float>(charProps.bearingX), static_cast<float>(charProps.bearingY)),
                glm::vec2(static_cast<float>(charProps.bearingX), static_cast<float>(charProps.bearingY - charProps.height)),
                glm::vec2(static_cast<float>(bmpWidth + charProps.bearingX), static_cast<float>(charProps.bearingY)),
                glm::vec2(static_cast<float>(bmpWidth + charProps.bearingX), static_cast<float>(charProps.bearingY - charProps.height))
            };

            glm::vec2 textureCoordinates[] =
            {
                glm::vec2(static_cast<float>(currentPixelPositionCol) / static_cast<float>(CHARACTERS_TEXTURE_SIZE), static_cast<float>(currentPixelPositionRow + bmpHeight) / static_cast<float>(CHARACTERS_TEXTURE_SIZE)),
                glm::vec2(static_cast<float>(currentPixelPositionCol) / static_cast<float>(CHARACTERS_TEXTURE_SIZE), static_cast<float>(currentPixelPositionRow) / static_cast<float>(CHARACTERS_TEXTURE_SIZE)),
                glm::vec2(static_cast<float>(currentPixelPositionCol + bmpWidth) / static_cast<float>(CHARACTERS_TEXTURE_SIZE), static_cast<float>(currentPixelPositionRow + bmpHeight) / static_cast<float>(CHARACTERS_TEXTURE_SIZE)),
                glm::vec2(static_cast<float>(currentPixelPositionCol + bmpWidth) / static_cast<float>(CHARACTERS_TEXTURE_SIZE), static_cast<float>(currentPixelPositionRow) / static_cast<float>(CHARACTERS_TEXTURE_SIZE))
            };

            for (int i = 0; i < 4; i++)
            {
                _vbo.addRawData(&vertices[i], sizeof(glm::vec2));
                _vbo.addRawData(&textureCoordinates[i], sizeof(glm::vec2));
            }

            charProps.renderIndex = currentRenderIndex;
            charProps.textureIndex = static_cast<int>(_textures.size());
            currentPixelPositionCol += bmpWidth + 1;
            currentRenderIndex += 4;
            c++;
        }
    }
    
    // If there is a leftover texture after preparing the characters, add it to the list of textures
    if (currentPixelPositionRow > 0 || currentPixelPositionCol > 0) {
        finalizeTexture(false);
    }

    _vbo.uploadDataToGPU(GL_STATIC_DRAW);
    // Setup vertex positions pointers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2)*2, reinterpret_cast<void*>(0));

    // Setup texture coordinates pointers
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2)*2, reinterpret_cast<void*>(sizeof(glm::vec2)));

    // Now we're done with loading, release FreeType structures
    FT_Done_Face(freeTypeFace);
    FT_Done_FreeType(freeTypeLibrary);

    _isLoaded = true;
    return true;
}

void FreeTypeFont::printInternal(int x, int y, const std::string& text, int pixelSize) const
{
    // Don't print, if the font hasn't been loaded successfully
    if (!_isLoaded) {
        return;
    }

    glDisable(GL_DEPTH_TEST);
    glDepthMask(0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto& shaderProgram = getFreetypeFontShaderProgram();
    shaderProgram.useProgram();
    shaderProgram[ShaderConstants::projectionMatrix()] = MatrixManager::getInstance().getOrthoProjectionMatrix();
    shaderProgram[ShaderConstants::color()] = _color;

    getFreetypeFontSampler().bind();
    shaderProgram[ShaderConstants::sampler()] = 0;

    glm::vec2 currentPos(x, y);
    const auto usedPixelSize = pixelSize == -1 ? _pixelSize : pixelSize;
    auto lastBoundTextureIndex = -1;
    const auto scale = static_cast<float>(usedPixelSize) / static_cast<float>(_pixelSize);

    glBindVertexArray(_vao);
    for(const auto& c : text)
    {
        if (c == '\n' || c == '\r')
        {
            currentPos.x = static_cast<float>(x);
            currentPos.y -= static_cast<float>(usedPixelSize);
            continue;
        }

        // If we somehow stumble upon unknown character, ignore it
        if (_characterProperties.count(c) == 0) {
            continue;
        }

        const auto& props = _characterProperties.at(c);
        if (props.renderIndex != -1)
        {
            if (lastBoundTextureIndex != props.textureIndex)
            {
                lastBoundTextureIndex = props.textureIndex;
                _textures.at(props.textureIndex)->bind();
            }

            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(currentPos.x, currentPos.y, 0.0f));
            model = glm::scale(model, glm::vec3(scale, scale, 1.0f));
            shaderProgram[ShaderConstants::modelMatrix()] = model;
            glDrawArrays(GL_TRIANGLE_STRIP, props.renderIndex, 4);
        }

        currentPos.x += props.advanceX*scale;
    }

    glDisable(GL_BLEND);
    glDepthMask(1);
    glEnable(GL_DEPTH_TEST);
}

int FreeTypeFont::getTextWidth(const std::string& text, int pixelSize) const
{
    float result = 0.0f;
    float rowWidth = 0.0f;
    const auto usedPixelSize = pixelSize == -1 ? _pixelSize : pixelSize;
    const auto scale = static_cast<float>(usedPixelSize) / static_cast<float>(_pixelSize);

    // TODO: would be nice to handle invalid characters here as well
    for (int i = 0; i < static_cast<int>(text.length()); i++)
    {
        if (text[i] == '\n' || text[i] == '\r') {
            continue;
        }

        bool lastCharacterInRow = i == text.length() - 1 || text[i + 1] == '\n' || text[i + 1] == '\r';
        const auto& props = _characterProperties.at(text[i]);
        if (!lastCharacterInRow)
        {
            rowWidth += props.advanceX*scale;
            continue;
        }

        // Handle last character in a row in a special way + update the result
        rowWidth += (props.bearingX + props.width)*scale;
        result = std::max(result, rowWidth);
        rowWidth = 0.0f;
    }

    // Return ceiling of result
    return static_cast<int>(ceil(result));
}

int FreeTypeFont::getTextHeight(int pixelSize) const
{
    const auto usedPixelSize = pixelSize == -1 ? _pixelSize : pixelSize;
    const auto scale = static_cast<float>(usedPixelSize) / static_cast<float>(_pixelSize);

    return static_cast<int>(ceil(usedPixelSize * scale));
}

void FreeTypeFont::setTextColor(const glm::vec4& color)
{
    _color = color;
}

ShaderProgram& FreeTypeFont::getFreetypeFontShaderProgram() const
{
    return ShaderProgramManager::getInstance().getShaderProgram(FREETYPE_FONT_PROGRAM_KEY);
}

const Sampler& FreeTypeFont::getFreetypeFontSampler() const
{
    return SamplerManager::getInstance().getSampler(FREETYPE_FONT_SAMPLER_KEY);
}

void FreeTypeFont::deleteFont()
{
    if (!_isLoaded) {
        return;
    }

    _textures.clear();
    _characterProperties.clear();
    _characterRanges.clear();

    _vbo.deleteVBO();
    glDeleteVertexArrays(1, &_vao);
    
    _isLoaded = false;
}
