#pragma once

#include <cassert>
#include <cstddef>
#include <type_traits>

#include "besm-666/exec/icsr.hpp"
#include "besm-666/riscv-types.hpp"
#include "besm-666/util/bit-magic.hpp"

namespace besm::exec {

enum class CSRFieldSpec { WPRI, WLRL, WARL, UNCHECKED };

using CSRFieldValidator = bool (*)(RV64UDWord);

class CSRFieldLike {};

template <RV64UDWord Mask, CSRFieldSpec Spec, CSRFieldValidator Val,
          RV64UDWord ResetValue = 0>
class CSRField : public CSRFieldLike {
public:
    // implication implementation
    static_assert(!(Spec != CSRFieldSpec::UNCHECKED &&
                    Spec != CSRFieldSpec::WPRI) ||
                  Val != nullptr);

    bool set(RV64UDWord value) noexcept;
    RV64UDWord get() const noexcept;

    ~CSRField() = default;

    explicit CSRField(RV64UDWord &rawRegister);

    bool extract(RV64UDWord registerValue) noexcept;
    void reset() const noexcept;

private:
    RV64UDWord &rawRegister_;
};

template <RV64UDWord Mask, RV64UDWord ResetValue = 0>
using CSRUncheckedField =
    CSRField<Mask, CSRFieldSpec::UNCHECKED, nullptr, ResetValue>;

template <RV64UDWord Mask>
using CSRWPRIField = CSRField<Mask, CSRFieldSpec::WPRI, nullptr>;

template <RV64UDWord Mask, CSRFieldValidator Validator,
          RV64UDWord ResetValue = 0>
using CSRWLRLField = CSRField<Mask, CSRFieldSpec::WLRL, Validator, ResetValue>;

template <RV64UDWord Mask, CSRFieldValidator Validator,
          RV64UDWord ResetValue = 0>
using CSRWARLField = CSRField<Mask, CSRFieldSpec::WARL, Validator, ResetValue>;

template <RV64UDWord Mask, CSRFieldSpec Spec, CSRFieldValidator Val,
          RV64UDWord ResetValue>
CSRField<Mask, Spec, Val, ResetValue>::CSRField(RV64UDWord &rawRegister)
    : rawRegister_(rawRegister) {}

template <RV64UDWord Mask, CSRFieldSpec Spec, CSRFieldValidator Val,
          RV64UDWord ResetValue>
bool CSRField<Mask, Spec, Val, ResetValue>::extract(
    RV64UDWord registerValue) noexcept {
    return this->set(util::ExtractMasked<RV64UDWord, Mask>(registerValue));
}

template <RV64UDWord Mask, CSRFieldSpec Spec, CSRFieldValidator Val,
          RV64UDWord ResetValue>
RV64UDWord CSRField<Mask, Spec, Val, ResetValue>::get() const noexcept {
    if constexpr (Spec == CSRFieldSpec::WPRI) {
        return 0;
    } else {
        return util::ExtractMasked<RV64UDWord, Mask>(rawRegister_);
    }
}

template <RV64UDWord Mask, CSRFieldSpec Spec, CSRFieldValidator Val,
          RV64UDWord ResetValue>
bool CSRField<Mask, Spec, Val, ResetValue>::set(RV64UDWord value) noexcept {
    if constexpr (Spec == CSRFieldSpec::WLRL) {
        if (!Val(value)) {
            return false;
        }
    } else if constexpr (Spec == CSRFieldSpec::WPRI) {
        return true;
    } else if constexpr (Spec == CSRFieldSpec::WARL) {
        if (!Val(value)) {
            return true;
        }
    }

    rawRegister_ = util::InsertMasked<RV64UDWord, Mask>(value, rawRegister_);
    return true;
}

template <RV64UDWord Mask, CSRFieldSpec Spec, CSRFieldValidator Val,
          RV64UDWord ResetValue>
void CSRField<Mask, Spec, Val, ResetValue>::reset() const noexcept {
    rawRegister_ = util::ExtractMasked<RV64UDWord, Mask>(ResetValue);
}

template <typename FieldType, typename... OtherFields>
class CSRStructureUnroller : public CSRStructureUnroller<OtherFields...>,
                             public FieldType {
public:
    static_assert(std::is_base_of_v<CSRFieldLike, FieldType>);

    CSRStructureUnroller(RV64UDWord &rawRegister)
        : CSRStructureUnroller<OtherFields...>(rawRegister),
          FieldType(rawRegister) {}
    ~CSRStructureUnroller() = default;

    inline bool extract(RV64UDWord value) {
        return FieldType::extract(value) &&
               CSRStructureUnroller<OtherFields...>::extract(value);
    }
    inline void reset() {
        FieldType::reset();
        CSRStructureUnroller<OtherFields...>::reset();
    }
};

template <typename FieldType>
class CSRStructureUnroller<FieldType> : public FieldType {
public:
    static_assert(std::is_base_of_v<CSRFieldLike, FieldType>);

    CSRStructureUnroller(RV64UDWord &rawField) noexcept : FieldType(rawField) {}
    ~CSRStructureUnroller() = default;

    inline bool extract(RV64UDWord value) { return FieldType::extract(value); }
    inline void reset() { FieldType::reset(); }
};

template <typename... Fields>
class CSRStructure : public CSRStructureUnroller<Fields...>, public ICSR {
public:
    CSRStructure(CSRF &csrf, ICSR::Id id)
        : CSRStructureUnroller<Fields...>(value_), ICSR(csrf, id) {
        this->reset();
    }

    void reset() { CSRStructureUnroller<Fields...>::reset(); }

    bool write(RV64UDWord value) noexcept override final {
        bool e = CSRStructureUnroller<Fields...>::extract(value);
        this->onUpdate();
        return e;
    }

    RV64UDWord read() const noexcept override final { return value_; }

    template <typename FieldType> bool set(RV64UDWord value) noexcept {
        bool e = static_cast<FieldType *>(this)->set(value);
        this->onUpdate();
        return e;
    }

    template <typename FieldType> RV64UDWord get() const noexcept {
        return static_cast<FieldType const *>(this)->get();
    }

protected:
    void onUpdate() noexcept override {}

private:
    RV64UDWord value_;
};

} // namespace besm::exec
