#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>

const int KEY_LENGTH = 8;
const int FIRST_SPACE_POS = 1;
const int SECOND_SPACE_POS = 5;
const int ASCII_SIZE = 256;
const int VALUE_LENGTH = 64;

using TKey = std::string;
using TValue = std::string;
using TObject = std::pair<TKey, TValue>;

void RadixSort(std::vector<TObject>& objects) {
    if (objects.empty()) {
        return;
    }

    const size_t size = objects.size();
    std::vector<size_t> indices(size);
    std::vector<size_t> tempIndices(size);
    std::array<int, ASCII_SIZE> count{};
    
    for (size_t i = 0; i < size; ++i) {
        indices[i] = i;
    }

    for (int digitPos = KEY_LENGTH - 1; digitPos >= 0; --digitPos) {
        if (digitPos == FIRST_SPACE_POS || digitPos == SECOND_SPACE_POS) {
            continue;
        }

        count.fill(0);

        for (size_t i = 0; i < size; ++i) {
            ++count[static_cast<unsigned char>(objects[indices[i]].first[digitPos])];
        }

        for (int i = 1; i < ASCII_SIZE; ++i) {
            count[i] += count[i - 1];
        }

        for (int i = static_cast<int>(size) - 1; i >= 0; --i) {
            unsigned char digit = static_cast<unsigned char>(
                objects[indices[i]].first[digitPos]);
            tempIndices[--count[digit]] = indices[i];
        }

        indices.swap(tempIndices);
    }

    std::vector<TObject> result;
    result.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        result.emplace_back(std::move(objects[indices[i]]));
    }
    objects.swap(result);
}

int main() {
    std::vector<TObject> objects;
    
    TKey key;
    TValue value;

    while (std::getline(std::cin, key, '\t') && std::getline(std::cin, value)) {
        if (!value.empty() && value.back() == '\r') {
            value.pop_back();
        }
        
        objects.emplace_back(std::move(key), std::move(value));
    }

    RadixSort(objects);

    for (const TObject& obj : objects) {
        std::cout << obj.first << '\t' << obj.second << '\n';
    }

    return 0;
}