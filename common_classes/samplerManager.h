#pragma once

#include <string>
#include <map>
#include <memory>

#include <glad/glad.h>

#include "sampler.h"

class SamplerManager
{
public:
	SamplerManager(const SamplerManager&) = delete;
	void operator=(const SamplerManager&) = delete;

	/** \brief  Gets the one and only instance of the sampler manager.
	*   \return Sampler manager instance.
	*/
	static SamplerManager& getInstance();

	/** \brief  Creates new sampler and stores it with specified key.
	*   \param  key Key to store sampler with
	*   \return Sampler instance with specified key.
	*/
	Sampler& createSampler(const std::string& key, const MagnificationFilter magnificationFilter, const MinificationFilter minificationFilter);

	/** \brief  Retrieves sampler with a specified key.
	*   \param  key Key to restore sampler from
	*   \return Sampler instance from a specified key.
	*/
	Sampler& getSampler(const std::string& key) const;

	/** \brief  Deletes all the samplers and clears the sampler cache. */
	void clearSamplerCache();

private:
	SamplerManager() {}

	/** \brief  Checks, if sampler with specified key exists.
	*   \param  key Sampler key to check existence of
	*   \return True if sampler exists, or false otherwise.
	*/
	bool containsSampler(const std::string& key) const;

	std::map<std::string, std::unique_ptr<Sampler>> _samplerCache; //!< Sampler cache - stores samplers within their keys in std::map
};