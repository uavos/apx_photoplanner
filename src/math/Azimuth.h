#pragma once

#include <math.h>

namespace aero_photo {

template <typename TDegreeType>
struct DegreeTraits {
    static constexpr TDegreeType Zero = {};
    static constexpr TDegreeType MaxUnique = {360};
};

// TAzimuth area of value [-180; 180]
template <typename TAzimuthType = qreal>
class TAzimuth {
public:
    using ValueType = TAzimuthType;
    using AzimuthTraits = DegreeTraits<ValueType>;

    TAzimuth() {}
    TAzimuth(TAzimuthType degree) : degree_(NormalizeValue(degree)) {
    }
    TAzimuth(const TAzimuth &other) : degree_(other.degree_) {
    }
    TAzimuth(TAzimuth &&other) : degree_(other.degree_) {
        other.degree_ = AzimuthTraits::Zero;
    }

    TAzimuth &operator= (const TAzimuth &other) {
        degree_ = other.degree_;
        return *this;
    }
    TAzimuth &operator= (TAzimuthType degree) {
        degree_ = NormalizeValue(degree);
        return *this;
    }

    TAzimuth &operator+= (const TAzimuth &other) {
        degree_ = NormalizeValue(degree_ + other.degree_);
        return *this;
    }
    TAzimuth &operator-= (const TAzimuth &other) {
        degree_ = NormalizeValue(degree_ - other.degree_);
        return *this;
    }

    operator ValueType() const { return degree_; }

private:

    inline static ValueType NormalizeValue(ValueType degree) {
        degree = std::remainder(degree, AzimuthTraits::MaxUnique);
        if (degree == 180)
            return -180;
        return degree;
    }
    ValueType degree_ = AzimuthTraits::Zero;
};

template <typename TAzimuthType>
TAzimuth<TAzimuthType> operator + (const TAzimuth<TAzimuthType> &lft, const TAzimuth<TAzimuthType> &rht) {
    TAzimuth<TAzimuthType> ret(lft);
    return ret+=rht;
}

template <typename TAzimuthType>
TAzimuth<TAzimuthType> operator - (const TAzimuth<TAzimuthType> &lft, const TAzimuth<TAzimuthType> &rht) {
    TAzimuth<TAzimuthType> ret(lft);
    return ret-=rht;
}

using Azimuth =  TAzimuth<qreal>;

} // aero_photo
