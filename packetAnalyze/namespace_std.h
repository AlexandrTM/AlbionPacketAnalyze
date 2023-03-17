#ifndef NAMESPACE_STD_H
#define NAMESPACE_STD_H

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

    template<class T>
    constexpr const T& clamp(const T& v, const T& lo, const T& hi)
    {
        return clamp(v, lo, hi, less{});
    }

    template<class T, class Compare>
    constexpr const T& clamp(const T& v, const T& lo, const T& hi, Compare comp)
    {
        return comp(v, lo) ? lo : comp(hi, v) ? hi : v;
    }

    float binToFloat(uint32_t value);
}

#endif