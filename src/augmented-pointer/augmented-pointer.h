#pragma once
#include <cstdint>
#include <assert.h>

/**
 * @brief Augmented pointer that allows data bits to be inserted into a pointer.
 * 
 * @tparam Ptr_ pointer type to augment.
 * @tparam Bits_ number of data bits to store in the pointer.
 */
template <typename Ptr_, uintptr_t Bits_>
class AugmentedPointer {
public:
    using ptr_type = Ptr_;

private:
    static constexpr uintptr_t bits_ = Bits_;

    static_assert((sizeof(ptr_type) == 4) || (sizeof(ptr_type) == 8), 
                  "Pointer must be either 32 bits or 64 bits");
    static_assert(((sizeof(ptr_type) == 4) && (bits_ <= 5)) || (sizeof(ptr_type) != 4), 
                  "Too many control bits for 32 bit pointer.");
    static_assert(((sizeof(ptr_type) == 8) && (bits_ <= 6)) || (sizeof(ptr_type) != 8), 
                  "Too many control bits for 64 bit pointer.");
private:

    // Mask that excludes the control bits of a pointer.
	static constexpr uintptr_t ptr_mask_ = ~static_cast<uintptr_t>((1 << (bits_ + 1)) - 1);

	// Mask that excludes the pointer bits of a pointer.
	static constexpr uintptr_t bit_mask_ = static_cast<uintptr_t>((1 << (bits_ + 1)) - 1);

private:
    ptr_type ptr_;

private:
    /**
     * @brief Tests if a pointer is aligned enough.15
     * 
     * @param pointer to test.
     * @return true if the pointer is aligned enough.
     * @return false if the pointer is not aligned enough.
     */
    inline bool is_clean_ptr(ptr_type ptr) const noexcept(NEX) {
        return !reinterpret_cast<ptr_type>(
			reinterpret_cast<uintptr_t>(ptr) & bit_mask_
		);
    }

public:
    /**
     * @brief Construct a new Augmented Pointer object.
     * 
     * @param pointer to wrap.
     */
    AugmentedPointer(ptr_type ptr) {
        set_ptr(ptr);
    }

    /**
     * @brief Set the pointer and unset all the control bits.
     * 
     * @param pointer to store.
     */
    inline void set_ptr(ptr_type ptr) noexcept(NEX) {
        assert(is_clean_ptr(ptr));
        ptr_ = ptr;
    }

    /**
     * @brief Get the pointer without the control bits.
     * 
     * @return pointer that is stored. 
     */
    inline ptr_type get_ptr() const noexcept(NEX) {
        return reinterpret_cast<ptr_type>(
			reinterpret_cast<uintptr_t>(ptr_) & ptr_mask_
		);
    }

    /**
     * @brief Sets a given bit.
     * 
     * @param bit to set.
     */
    inline void set_bit(uintptr_t bit) noexcept(NEX) {
        assert((bit >= 0) && (bit <= 4));
        ptr_ = reinterpret_cast<ptr_type>(
			reinterpret_cast<uintptr_t>(ptr_) | (1 << bit)
		);
    }

    /**
     * @brief Unsets a given bit.
     * 
     * @param bit to unset.
     */
    inline void unset_bit(uintptr_t bit) noexcept(NEX) {
        assert((bit >= 0) && (bit <= 4));
        ptr_ = reinterpret_cast<ptr_type>(
			reinterpret_cast<uintptr_t>(ptr_) & ~(1 << bit)
		);
    }

    /**
     * @brief Tests if a bit is set 
     * 
     * @param bit to test.
     * @return true if the bit is set.
     * @return false if the bit is unset.
     */
    inline bool is_set_bit(uintptr_t bit) const noexcept(NEX) {
        assert((bit >= 0) && (bit <= 4));
        return reinterpret_cast<ptr_type>(
			reinterpret_cast<uintptr_t>(ptr_) & (1 << bit)
		);
    }
};