#pragma once

namespace std
{
    template <class T>
    bool isElementInVector(std::vector<T>& vector, T element)
    {
        if (std::find(std::begin(vector), std::end(vector), element) != std::end(vector)) {
            return true;
        }
        else {
            return false;
        }
    }

    template <class T>
    size_t findElementIndex(std::vector<T>& vector, T element)
    {
        return std::distance(vector.begin(), std::find(vector.begin(), vector.end(), element));
    }

    template <class T>
    T findElementInVector(std::vector<T>& vector, T element)
    {
        return vector[std::findElementIndex(vector, element)];
    }
}
