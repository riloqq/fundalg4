//
// Created by Олеся on 27.03.2025.
//

#ifndef FUNDS_4_1_CONTAINER_HPP
#define FUNDS_4_1_CONTAINER_HPP

#include <iostream>

template <typename T, std::size_t N>
class Container {
public:
    Container() = default;
    Container(const Container& other) = default;
    Container& operator=(const Container& other) = default;

    virtual ~Container() = default;

    virtual T* begin() = 0;
    virtual const T* cbegin() const = 0;
    virtual T* end() = 0;
    virtual const T* cend() const = 0;

    virtual bool operator==(const Container& other) const = 0;
    virtual bool operator!=(const Container& other) const = 0;

    virtual std::size_t size() const = 0;
    virtual std::size_t max_size() const = 0;
    virtual bool empty() const = 0;
};

template <typename T, std::size_t N>
class Array final : public Container<T, N> {
    T m_data[N];

public:
    Array() = default;
    Array(std::initializer_list<T> init) {
        std::copy(init.begin(), init.end(), m_data);
    }
    ~Array() override = default;
    Array(const Array& other) {
        std::copy(other.cbegin(), other.cend(), this->begin());
    }
    Array(Array&& other) noexcept {
        std::move(other.begin(), other.end(), this->begin());
        std::fill(other.begin(),other.end(), T{});
    }
    Array& operator=(const Array& other) {
        if (this != &other) {
            std::copy(other.cbegin(), other.cend(), this->begin());
        }
        return *this;
    }
    Array& operator=(Array&& other) noexcept {
        if (this != &other) {
            std::move(other.begin(), other.end(), this->begin());
        }
        return *this;
    }
    T& at(std::size_t index) {
        if (index >= N) {
            throw std::out_of_range("Index out of range");
        }
        return m_data[index];
    }
    const T& at(std::size_t index) const {
        if (index >= N) {
            throw std::out_of_range("Index out of range");
        }
        return m_data[index];
    }
    T& operator[](std::size_t index) {
        return m_data[index];
    }
    const T& operator[](std::size_t index) const{
        return m_data[index];
    }
    T& front() {
        return m_data[0];
    }
    const T& front() const{
        return m_data[0];
    }
    T& back() {
        return m_data[N - 1];
    }
    const T& back() const{
        return m_data[N - 1];
    }
    T* data() noexcept {
        return m_data;
    }
    const T* data() const noexcept {
        return m_data;
    }
    T* begin() noexcept override {
        return m_data;
    }
    const T* cbegin() const noexcept override {
        return m_data;
    }
    T* end() noexcept override {
        return m_data + N;
    }
    const T* cend() const noexcept override {
        return m_data + N;
    }
    std::reverse_iterator<T*> rbegin() noexcept {
        return std::reverse_iterator<T*>(end());
    }
    std::reverse_iterator<const T*> crbegin() const noexcept {
        return std::reverse_iterator<T*>(cend());
    }
    std::reverse_iterator<T*> rend() noexcept {
        return std::reverse_iterator<T*>(begin());
    }
    std::reverse_iterator<const T*> crend() const noexcept {
        return std::reverse_iterator<T*>(cbegin());
    }
    std::size_t size() const noexcept override {
        return N;
    }
    std::size_t max_size() const noexcept override {
        return N;
    }
    bool empty() const noexcept override {
        return N == 0;
    }
    void fill(const T& val) {
        std::fill(begin(), end(), val);
    }
    void swap(Array& other) noexcept {
        std::swap_ranges(begin(), end(), other.begin());
    }
    bool operator==(const Container<T, N>& other) const override {
        const Array* otherArray = dynamic_cast<const Array*>(&other);
        if (!otherArray) {
            return false;
        }
        return std::equal(cbegin(), cend(), otherArray->cbegin(), otherArray->cend());
    }
    bool operator!=(const Container<T, N>& other) const override {
        return !(*this == other);
    }
    bool operator<(const Array& other) const {
        return std::lexicographical_compare(cbegin(), cend(), other.cbegin(), other.cend());
    }
    bool operator<=(const Array& other) const {
        return (*this == other) || (*this < other);
    }
    bool operator>(const Array& other) const {
        return other < *this;
    }
    bool operator>=(const Array& other) const {
        return (other < *this) || (*this == other);
    }
    auto operator<=>(const Array& other) const {
        return std::lexicographical_compare_three_way(cbegin(), cend(), other.cbegin(), other.cend());
    }
};

#endif //FUNDS_4_1_CONTAINER_HPP
