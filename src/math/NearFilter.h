#pragma once

#include "Array2d.h"

namespace aero_photo {

template <typename T>
class NearFilter {
public:
    explicit NearFilter(size_t filterSize) : filterSize_(filterSize) { }

    Array2D<T> operator() (const Array2D<T> &inArray) {

        Array2D<T> workArray(inArray.Rows() + 2*filterSize_, inArray.Cols() + 2*filterSize_);
        auto initWorkItem = [this, &workArray](auto iRow, auto iCol, T item) {
            workArray.Item(iRow + filterSize_, iCol + filterSize_) = item;
        };
        inArray.IterateItems(initWorkItem);

        Array2D<T> outArray(inArray.Rows(), inArray.Cols());
        auto fillOutItem = [this, &workArray] (auto iRow, auto iCol) {
            for (size_t iFilterRow = iRow; iFilterRow < iRow + 2*filterSize_ + 1; iFilterRow++)
                for (size_t iFilterCol = iCol; iFilterCol < iCol + 2*filterSize_ + 1; iFilterCol++) {
                    if (workArray.Item(iFilterRow, iFilterCol))
                        return 1;
                }
            return 0;
        };
        outArray.SetItems(fillOutItem);

        return outArray;
    }

private:
    const size_t filterSize_;
};

}
