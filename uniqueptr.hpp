#ifndef UPTR_PTR_HPP
#define UPTR_PTR_HPP

#include <utility>
#include <stdexcept>

namespace my_smart_ptr {

    template <typename T>
    class UniquePtr {
    public:
        explicit UniquePtr(T* ptr = nullptr) : ptr_(ptr) {}
        ~UniquePtr() { delete ptr_; }

        UniquePtr(const UniquePtr&) = delete;
        UniquePtr& operator=(const UniquePtr&) = delete;

        UniquePtr(UniquePtr&& other) noexcept : ptr_(other.ptr_) {
            other.ptr_ = nullptr;
        }

        UniquePtr& operator=(UniquePtr&& other) noexcept {
            if (this != &other) {
                delete ptr_;
                ptr_ = other.ptr_;
                other.ptr_ = nullptr;
            }
            return *this;
        }

        T* get() const { return ptr_; }
        T* release() {
            T* tmp = ptr_;
            ptr_ = nullptr;
            return tmp;
        }
        void reset(T* ptr = nullptr) {
            delete ptr_;
            ptr_ = ptr;
        }
        void swap(UniquePtr& other) {
            T* tmp = ptr_;
            ptr_ = other.ptr_;
            other.ptr_ = tmp;
        }

        T& operator*() const {
            if (!ptr_) throw std::runtime_error("Dereferencing null pointer");
            return *ptr_;
        }

        T* operator->() const {
            if (!ptr_) throw std::runtime_error("Dereferencing null pointer");
            return ptr_;
        }

        explicit operator bool() const { return ptr_ != nullptr; }

    private:
        T* ptr_;
    };

    template <typename T>
    class UniquePtr<T[]> {
    public:
        explicit UniquePtr(T* ptr = nullptr) : ptr_(ptr) {}
        ~UniquePtr() { delete[] ptr_; }

        UniquePtr(const UniquePtr&) = delete;
        UniquePtr& operator=(const UniquePtr&) = delete;

        UniquePtr(UniquePtr&& other) noexcept : ptr_(other.ptr_) {
            other.ptr_ = nullptr;
        }
        UniquePtr& operator=(UniquePtr&& other) noexcept {
            if (this != &other) {
                delete[] ptr_;
                ptr_ = other.ptr_;
                other.ptr_ = nullptr;
            }
            return *this;
        }

        T* get() const { return ptr_; }
        T* release() {
            T* tmp = ptr_;
            ptr_ = nullptr;
            return tmp;
        }
        void reset(T* ptr = nullptr) {
            delete[] ptr_;
            ptr_ = ptr;
        }
        void swap(UniquePtr& other) {
            T* tmp = ptr_;
            ptr_ = other.ptr_;
            other.ptr_ = tmp;
        }

        T& operator[](std::size_t index) const {
            return ptr_[index];
        }

        explicit operator bool() const { return ptr_ != nullptr; }

    private:
        T* ptr_;
    };

    template<typename T>
    UniquePtr<T> MakeUnique() {
        return UniquePtr<T>(new T());
    }

    template<typename T>
    std::enable_if_t<std::is_array<T>::value && std::extent<T>::value == 0, UniquePtr<T>>
    MakeUnique(std::size_t size) {
        using Elem = std::remove_extent_t<T>;
        return UniquePtr<T>(new Elem[size]());
    }

    template<typename T>
    std::enable_if_t<std::is_array_v<T> && std::extent_v<T> != 0, UniquePtr<T>>
    MakeUnique() {
        using Elem = std::remove_extent_t<T>;
        constexpr std::size_t size = std::extent_v<T>;
        return UniquePtr<T>(new Elem[size]());
    }

} // namespace my_smart_ptr

#endif // UPTR_PTR_HPP
