/**
 * @file vectorMap.h
 * @brief Class which allows for data to be accessed both as a vector and as an (unordered) map.
 *
 * Inspired by ChatGPT, an AI language model developed by:
 *     OpenAI. (2023). ChatGPT (Version 3.5) [Computer software].
 *     Retrieved from https://openai.com/gpt-3/
 *     Accessed on: 2023 April 03-07
 * 
 * @author Adam King
 * @date 2023-04-19
 */

#pragma once

#include <cstddef>
#include <unordered_map>
#include <vector>

namespace vm {

	template<typename typKeyType, typename typValueType>
	class clsVectorMap {

	public:
		clsVectorMap() {}

		void
			insert(
				typKeyType const typKey,
				typValueType const typValue
			) {
			size_t const sizVctIdx = vctInternal.size();
			vctInternal.push_back(typValue);
			mapInternal.insert({ typKey, sizVctIdx });
		}

		// key count (map)
		size_t
			count(
				typKeyType const typKey
			) const {
			return mapInternal.count(typKey);
		}

		// element count (vector)
		size_t
			size(
				void
			) const {
			return vctInternal.size();
		}

		// indexing
		typValueType
			operator[] (
				std::size_t const sizIndex
				) const {
			return vctInternal.at(sizIndex);
		}

		// lookup
		typValueType
			operator[] (
				typKeyType const typKey
				) const {
			return vctInternal.at(mapInternal.at(typKey));
		}

	private:
		std::unordered_map<typKeyType, size_t> mapInternal;
		std::vector<typValueType> vctInternal;

	};

}
