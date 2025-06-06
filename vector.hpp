
#include <iostream>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <compare>
#include <cstddef>
#include <algorithm>

#ifndef VECTOR_VECTOR_HPP
#define VECTOR_VECTOR_HPP

template <typename T>
class Container {
public:
    Container() = default;
    Container(const Container& other) = default;
    virtual Container& operator=(const Container& other) = 0;
    virtual ~Container() = 0;

    virtual bool operator==(const Container& other) const = 0;
    virtual bool operator!=(const Container& other) const = 0;

    virtual std::size_t size() const = 0;
    virtual std::size_t max_size() const = 0;
    virtual bool empty() const = 0;
};
template <typename T>
Container<T>:: ~Container() = default;

namespace my_container {

    template<typename T>
    class Vector : public Container<T> {
    public:
        Vector() = default;

        Vector(const Vector& other)
                : size_(other.size_), capacity_(other.capacity_), data_(new T[capacity_]) {
            std::copy(other.data_, other.data_ + size_, data_);
        }

        Vector(Vector&& other) noexcept
                : size_(other.size_), capacity_(other.capacity_), data_(other.data_) {
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }

        Vector(std::initializer_list<T> init)
                : size_(init.size()), capacity_(init.size()), data_(new T[capacity_]) {
            std::copy(init.begin(), init.end(), data_);
        }

        ~Vector() override {
            delete[] data_;
        }

        Container<T>& operator=(const Container<T>& other) override {
            if (this != &other) {
                auto* vec = dynamic_cast<const Vector*>(&other);
                if (!vec) {
                    throw std::invalid_argument("Assigned Container must be of type Vector");
                }
                *this = *vec;
            }
            return *this;
        }


        Vector& operator=(const Vector& other) {
            if (this != &other) {
                T* new_data = new T[other.capacity_];
                std::copy(other.data_, other.data_ + other.size_, new_data);
                delete[] data_;
                data_ = new_data;
                size_ = other.size_;
                capacity_ = other.capacity_;
            }
            return *this;
        }

        Vector& operator=(Vector&& other) noexcept {
            if (this != &other) {
                delete[] data_;
                data_ = other.data_;
                size_ = other.size_;
                capacity_ = other.capacity_;
                other.data_ = nullptr;
                other.size_ = 0;
                other.capacity_ = 0;
            }
            return *this;
        }

        T& operator[](std::size_t index) {
            return data_[index];
        }

        const T& operator[](std::size_t index) const {
            return data_[index];
        }

        T& at(std::size_t index) {
            if (index >= size_) throw std::out_of_range("Index out of range");
            return data_[index];
        }

        const T& at(std::size_t index) const {
            if (index >= size_) throw std::out_of_range("Index out of range");
            return data_[index];
        }

        T& front() {
            return data_[0];
        }

        const T& front() const {
            return data_[0];
        }

        T& back() {
            return data_[size_ - 1];
        }

        const T& back() const {
            return data_[size_ - 1];
        }

        T* data() {
            return data_;
        }

        const T* data() const {
            return data_;
        }

        bool empty() const override {
            return size_ == 0;
        }

        std::size_t size() const override {
            return size_;
        }

        std::size_t capacity() const {
            return capacity_;
        }

        std::size_t max_size() const override {
            return static_cast<std::size_t>(-1) / sizeof(T);
        }

        void reserve(std::size_t new_cap) {
            if (new_cap > capacity_) {
                T* new_data = new T[new_cap];
                std::copy(data_, data_ + size_, new_data);
                delete[] data_;
                data_ = new_data;
                capacity_ = new_cap;
            }
        }

        void shrink_to_fit() {
            if (capacity_ > size_) {
                T* new_data = new T[size_];
                std::copy(data_, data_ + size_, new_data);
                delete[] data_;
                data_ = new_data;
                capacity_ = size_;
            }
        }

        void clear() {
            size_ = 0;
        }

        void push_back(const T& value) {
            if (size_ >= capacity_) reserve(capacity_ == 0 ? 1 : capacity_ * 2);
            data_[size_++] = value;
        }

        void pop_back() {
            if (size_ > 0) --size_;
        }

        void insert(std::size_t pos, const T& value) {
            if (pos > size_) throw std::out_of_range("Insert position out of range");
            if (size_ >= capacity_) reserve(capacity_ == 0 ? 1 : capacity_ * 2);
            for (std::size_t i = size_; i > pos; --i) {
                data_[i] = std::move(data_[i - 1]);
            }
            data_[pos] = value;
            ++size_;
        }

        void erase(std::size_t pos) {
            if (pos >= size_) throw std::out_of_range("Erase position out of range");
            for (std::size_t i = pos; i < size_ - 1; ++i) {
                data_[i] = std::move(data_[i + 1]);
            }
            --size_;
        }

        void resize(std::size_t count, const T& value = T()) {
            if (count > capacity_) reserve(count);
            if (count > size_) {
                std::fill(data_ + size_, data_ + count, value);
            }
            size_ = count;
        }

        void swap(Vector& other) noexcept {
            std::swap(data_, other.data_);
            std::swap(size_, other.size_);
            std::swap(capacity_, other.capacity_);
        }

        bool operator==(const Container<T>& other) const override {
            auto* p = dynamic_cast<const Vector*>(&other);
            if (!p || p->size_ != size_) return false;
            return std::equal(data_, data_ + size_, p->data_);
        }

        bool operator!=(const Container<T>& other) const override {
            return !(*this == other);
        }

        std::strong_ordering operator<=>(const Vector& other) const {
            if (auto cmp = size_ <=> other.size_; cmp != 0) return cmp;
            for (std::size_t i = 0; i < size_; ++i) {
                if (auto cmp = data_[i] <=> other.data_[i]; cmp != 0) return cmp;
            }
            return std::strong_ordering::equal;
        }


    private:
        std::size_t size_ = 0;
        std::size_t capacity_ = 0;
        T* data_ = nullptr;
    };

}  // namespace my_container

#endif //VECTOR_VECTOR_HPP
