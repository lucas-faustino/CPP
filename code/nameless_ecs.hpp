//MIT License

//Copyright (c) 2022 lucas-faustino

//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:

//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

#pragma once

#include <stdint.h>
#include <memory>
// really simple ecs mostly just brainstorming ideas. I don't suggest using it as is but
// I hope it gives you some ideas; probably has bugs.
namespace nameless::ecs {
    // make this larger if you need a larger max size
    using Int = uint16_t;

    constexpr auto COMPONENT_MAX = 16;
    // generates a unique id for each component type when get<T> is called
    struct TypeId {
    private:
        static const auto get_id() {
            static Int value = 0;
            return value++;
        }
    public:
        template<typename T>
        static const auto get = get_id();
    };

    struct Archetype {
        Int type_ids        [COMPONENT_MAX];
        Int type_sizes      [COMPONENT_MAX];
        Int type_ptr_offsets[COMPONENT_MAX];
        Int component_count;
        Int entity_size;
    };

    struct MemoryBlock {
        std::unique_ptr<uint8_t[]> ptr;
        size_t                     size;
        size_t                     capacity;

        inline MemoryBlock(const size_t capacity) :
            ptr(std::make_unique(capacity)),
            size(0),
            capacity(capacity) {}
    }

    struct Chunk {
        MemoryBlock                buffer;
        std::shared_ptr<Archetype> archetype;
        size_t                     entity_count;
    };

    template<typename... T>
    auto make_archetype() {
        const auto count = static_cast<Int>(sizeof...(T));
        const Int type_ids  [] = { TypeId::get<T>... };
        const Int type_sizes[] = { sizeof(T)... };
        auto archetype = Archetype{};
        for(Int i = 0; i < count; i++) {
            archetype.type_ids[i] = type_ids[i];
        }
        for(Int i = 0; i < count; i++) {
            archetype.type_sizes[i] = type_sizes[i];
        }

        Int offset = 0;
        for(Int i = 0; i < count; i++) {
            archetype.ptr_offset[i] = offset;
            offset += type_sizes[i];
        }
        
        archetype.component_count = count;
        archetype.entity_size     = ((sizeof(T)...) +);
        return archetype;
    }

    template<typename T>
    auto get_component(Chunk &chunk) {
        for(Int i = 0; i < chunk.archetype->component_count; i++) {
            if(archetype.type_ids[i] == TypeId::get<T>) {
                return reinterpret_cast<T*>(chunk.buffer.ptr + chunk.archetype->type_ptr_offsets[i]);
            }
        }
        return nullptr;
    }

    template<typename Action, typename... T>
    void for_each(Chunk &chunk, Action action) {
        for(Int i = 0; i < chunk.archetype->component_count; i++) {
            action(get_component<T>(chunk)...);
        }        
    }

    struct World {

    };  
}
