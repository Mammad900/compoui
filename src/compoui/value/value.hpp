#pragma once

#include <memory>

namespace CUI {

    namespace Value {
        template<typename T>
        class Value {
        public:
            virtual const T& get() = 0;
        };
    
        template<typename T>
        class FixedValue : public Value<T> {
        public:
            T value;
            FixedValue(T value) : value(value) {}
            const T& get() override {
                return value;
            }
        };
    
        template<typename T>
        class ReferenceValue : public Value<T> {
        public:
            T* value;
            ReferenceValue(T* value) : value(value) {}
            const T& get() override {
                return *value;
            }
        };
    }

    namespace Factories {
        template<typename T>
        inline std::shared_ptr<Value::FixedValue<T>> FixedValue(T value) {
            return std::make_shared<Value::FixedValue<T>>(value);
        }

        template<typename T>
        inline std::shared_ptr<Value::ReferenceValue<T>> ReferenceValue(T* value) {
            return std::make_shared<Value::ReferenceValue<T>>(value);
        }
    }

    template<typename T>
    class Val {
        std::shared_ptr<Value::Value<T>> ptr;

    public:
        Val(T value) : ptr(std::make_shared<Value::FixedValue<T>>(std::move(value))) {}
        template<typename U>
            requires std::derived_from<U, Value::Value<T>>
        Val(std::shared_ptr<U> ptr) : ptr(std::move(ptr)) {}

        const T& get() {
            return ptr->get();
        }
    };

    namespace Value {
        template<typename T>
        class LerpValue : public Value<T> {
        public:
            T a, b;
            Val<float> t;
            T lastVal;
            LerpValue(T a, T b, Val<float> t) : a(a), b(b), t(t) {}
            const T& get() override {
                const float &t = this->t.get();
                return lastVal = a * (1.f - t) + b * t;
            }
        };
        template<typename T>
        class LerpValue2 : public Value<T> {
        public:
            Val<T> a, b;
            Val<float> t;
            T lastVal;
            LerpValue2(Val<T> a, Val<T> b, Val<float> t) : a(a), b(b), t(t) {}
            const T& get() override {
                const float &t = this->t.get();
                return lastVal = a.get() * (1.f - t) + b.get() * t;
            }
        };
    }

    namespace Factories {
        template<typename T>
        inline std::shared_ptr<Value::LerpValue<T>> LerpValue(T a, T b, Val<float> t) {
            return std::make_shared<Value::LerpValue<T>>(a, b, t);
        }
        template<typename T>
        inline std::shared_ptr<Value::LerpValue2<T>> LerpValue2(Val<T> a, Val<T> b, Val<float> t) {
            return std::make_shared<Value::LerpValue2<T>>(a, b, t);
        }
    }
}