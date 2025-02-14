#include <torch/torch.h>
#include "core/conversion/converters/converter_util.h"
#include "core/conversion/converters/converters.h"
#include "core/util/prelude.h"
#include "core/util/trt_util.h"

namespace trtorch {
namespace core {
namespace conversion {
namespace converters {
namespace impl {
namespace {

auto cast_registrations TRTORCH_UNUSED =
    RegisterNodeConversionPatterns()
        .pattern(
            {"aten::to.dtype(Tensor self, int dtype, bool non_blocking=False, bool copy=False, int? memory_format=None) -> (Tensor)",
             [](ConversionCtx* ctx, const torch::jit::Node* n, args& args) -> bool {
               auto self = args[0].ITensorOrFreeze(ctx);
               auto output_dtype = args[1].unwrapToScalar().to<int64_t>();
               auto trt_dtype = util::ScalarTypeToTRTDataType(static_cast<at::ScalarType>(output_dtype));
               auto casted_itensor = castITensor(ctx, self, trt_dtype);
               auto output = ctx->AssociateValueAndTensor(n->outputs()[0], casted_itensor);
               LOG_DEBUG("[aten::to.dtype] Output tensor shape: " << output->getDimensions());

               return true;
             }})
        .pattern(
            {"aten::to.other(Tensor self, Tensor other, bool non_blocking=False, bool copy=False, int? memory_format=None) -> (Tensor)",
             [](ConversionCtx* ctx, const torch::jit::Node* n, args& args) -> bool {
               auto self = args[0].ITensorOrFreeze(ctx);
               nvinfer1::DataType other_dtype = args[1].ITensorOrFreeze(ctx)->getType();
               auto casted_itensor = castITensor(ctx, self, other_dtype);
               auto output = ctx->AssociateValueAndTensor(n->outputs()[0], casted_itensor);
               LOG_DEBUG("[aten::to.other] Output tensor shape: " << output->getDimensions());

               return true;
             }})
        .pattern(
            {"aten::to.prim_Device(Tensor(a) self, Device? device, int? dtype=None, bool non_blocking=False, bool copy=False) -> (Tensor(b|a))",
             [](ConversionCtx* ctx, const torch::jit::Node* n, args& args) -> bool {
               auto self = args[0].ITensorOrFreeze(ctx);
               if (args[2].isIValue() && !args[2].IValue()->isScalar()) {
                 auto output = ctx->AssociateValueAndTensor(n->outputs()[0], self);
                 LOG_DEBUG("[aten::to.prim_Device] Output tensor shape: " << output->getDimensions());
                 return true;
               }

               auto output_dtype = args[2].unwrapToScalar().to<int64_t>();
               auto trt_dtype = util::ScalarTypeToTRTDataType(static_cast<at::ScalarType>(output_dtype));
               auto casted_itensor = castITensor(ctx, self, trt_dtype);
               auto output = ctx->AssociateValueAndTensor(n->outputs()[0], casted_itensor);
               LOG_DEBUG("[aten::to.prim_Device] Output tensor shape: " << output->getDimensions());

               return true;
             }});
// clang-format on
} // namespace
} // namespace impl
} // namespace converters
} // namespace conversion
} // namespace core
} // namespace trtorch
