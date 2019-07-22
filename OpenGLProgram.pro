QT += quick
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        glm/detail/glm.cpp \
        main.cpp \
        openglrenderer.cpp \
        stb_image.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

PRECOMPILED_HEADER = glm_pch.h

HEADERS += \
    glm/common.hpp \
    glm/detail/_features.hpp \
    glm/detail/_fixes.hpp \
    glm/detail/_noise.hpp \
    glm/detail/_swizzle.hpp \
    glm/detail/_swizzle_func.hpp \
    glm/detail/_vectorize.hpp \
    glm/detail/compute_common.hpp \
    glm/detail/compute_vector_relational.hpp \
    glm/detail/qualifier.hpp \
    glm/detail/setup.hpp \
    glm/detail/type_float.hpp \
    glm/detail/type_half.hpp \
    glm/detail/type_mat2x2.hpp \
    glm/detail/type_mat2x3.hpp \
    glm/detail/type_mat2x4.hpp \
    glm/detail/type_mat3x2.hpp \
    glm/detail/type_mat3x3.hpp \
    glm/detail/type_mat3x4.hpp \
    glm/detail/type_mat4x2.hpp \
    glm/detail/type_mat4x3.hpp \
    glm/detail/type_mat4x4.hpp \
    glm/detail/type_quat.hpp \
    glm/detail/type_vec1.hpp \
    glm/detail/type_vec2.hpp \
    glm/detail/type_vec3.hpp \
    glm/detail/type_vec4.hpp \
    glm/exponential.hpp \
    glm/ext.hpp \
    glm/ext/matrix_clip_space.hpp \
    glm/ext/matrix_common.hpp \
    glm/ext/matrix_double2x2.hpp \
    glm/ext/matrix_double2x2_precision.hpp \
    glm/ext/matrix_double2x3.hpp \
    glm/ext/matrix_double2x3_precision.hpp \
    glm/ext/matrix_double2x4.hpp \
    glm/ext/matrix_double2x4_precision.hpp \
    glm/ext/matrix_double3x2.hpp \
    glm/ext/matrix_double3x2_precision.hpp \
    glm/ext/matrix_double3x3.hpp \
    glm/ext/matrix_double3x3_precision.hpp \
    glm/ext/matrix_double3x4.hpp \
    glm/ext/matrix_double3x4_precision.hpp \
    glm/ext/matrix_double4x2.hpp \
    glm/ext/matrix_double4x2_precision.hpp \
    glm/ext/matrix_double4x3.hpp \
    glm/ext/matrix_double4x3_precision.hpp \
    glm/ext/matrix_double4x4.hpp \
    glm/ext/matrix_double4x4_precision.hpp \
    glm/ext/matrix_float2x2.hpp \
    glm/ext/matrix_float2x2_precision.hpp \
    glm/ext/matrix_float2x3.hpp \
    glm/ext/matrix_float2x3_precision.hpp \
    glm/ext/matrix_float2x4.hpp \
    glm/ext/matrix_float2x4_precision.hpp \
    glm/ext/matrix_float3x2.hpp \
    glm/ext/matrix_float3x2_precision.hpp \
    glm/ext/matrix_float3x3.hpp \
    glm/ext/matrix_float3x3_precision.hpp \
    glm/ext/matrix_float3x4.hpp \
    glm/ext/matrix_float3x4_precision.hpp \
    glm/ext/matrix_float4x2.hpp \
    glm/ext/matrix_float4x2_precision.hpp \
    glm/ext/matrix_float4x3.hpp \
    glm/ext/matrix_float4x3_precision.hpp \
    glm/ext/matrix_float4x4.hpp \
    glm/ext/matrix_float4x4_precision.hpp \
    glm/ext/matrix_projection.hpp \
    glm/ext/matrix_relational.hpp \
    glm/ext/matrix_transform.hpp \
    glm/ext/quaternion_common.hpp \
    glm/ext/quaternion_double.hpp \
    glm/ext/quaternion_double_precision.hpp \
    glm/ext/quaternion_exponential.hpp \
    glm/ext/quaternion_float.hpp \
    glm/ext/quaternion_float_precision.hpp \
    glm/ext/quaternion_geometric.hpp \
    glm/ext/quaternion_relational.hpp \
    glm/ext/quaternion_transform.hpp \
    glm/ext/quaternion_trigonometric.hpp \
    glm/ext/scalar_common.hpp \
    glm/ext/scalar_constants.hpp \
    glm/ext/scalar_int_sized.hpp \
    glm/ext/scalar_relational.hpp \
    glm/ext/scalar_uint_sized.hpp \
    glm/ext/scalar_ulp.hpp \
    glm/ext/vector_bool1.hpp \
    glm/ext/vector_bool1_precision.hpp \
    glm/ext/vector_bool2.hpp \
    glm/ext/vector_bool2_precision.hpp \
    glm/ext/vector_bool3.hpp \
    glm/ext/vector_bool3_precision.hpp \
    glm/ext/vector_bool4.hpp \
    glm/ext/vector_bool4_precision.hpp \
    glm/ext/vector_common.hpp \
    glm/ext/vector_double1.hpp \
    glm/ext/vector_double1_precision.hpp \
    glm/ext/vector_double2.hpp \
    glm/ext/vector_double2_precision.hpp \
    glm/ext/vector_double3.hpp \
    glm/ext/vector_double3_precision.hpp \
    glm/ext/vector_double4.hpp \
    glm/ext/vector_double4_precision.hpp \
    glm/ext/vector_float1.hpp \
    glm/ext/vector_float1_precision.hpp \
    glm/ext/vector_float2.hpp \
    glm/ext/vector_float2_precision.hpp \
    glm/ext/vector_float3.hpp \
    glm/ext/vector_float3_precision.hpp \
    glm/ext/vector_float4.hpp \
    glm/ext/vector_float4_precision.hpp \
    glm/ext/vector_int1.hpp \
    glm/ext/vector_int1_precision.hpp \
    glm/ext/vector_int2.hpp \
    glm/ext/vector_int2_precision.hpp \
    glm/ext/vector_int3.hpp \
    glm/ext/vector_int3_precision.hpp \
    glm/ext/vector_int4.hpp \
    glm/ext/vector_int4_precision.hpp \
    glm/ext/vector_relational.hpp \
    glm/ext/vector_uint1.hpp \
    glm/ext/vector_uint1_precision.hpp \
    glm/ext/vector_uint2.hpp \
    glm/ext/vector_uint2_precision.hpp \
    glm/ext/vector_uint3.hpp \
    glm/ext/vector_uint3_precision.hpp \
    glm/ext/vector_uint4.hpp \
    glm/ext/vector_uint4_precision.hpp \
    glm/ext/vector_ulp.hpp \
    glm/fwd.hpp \
    glm/geometric.hpp \
    glm/glm.hpp \
    glm/gtc/bitfield.hpp \
    glm/gtc/color_space.hpp \
    glm/gtc/constants.hpp \
    glm/gtc/epsilon.hpp \
    glm/gtc/integer.hpp \
    glm/gtc/matrix_access.hpp \
    glm/gtc/matrix_integer.hpp \
    glm/gtc/matrix_inverse.hpp \
    glm/gtc/matrix_transform.hpp \
    glm/gtc/noise.hpp \
    glm/gtc/packing.hpp \
    glm/gtc/quaternion.hpp \
    glm/gtc/random.hpp \
    glm/gtc/reciprocal.hpp \
    glm/gtc/round.hpp \
    glm/gtc/type_aligned.hpp \
    glm/gtc/type_precision.hpp \
    glm/gtc/type_ptr.hpp \
    glm/gtc/ulp.hpp \
    glm/gtc/vec1.hpp \
    glm/gtx/associated_min_max.hpp \
    glm/gtx/bit.hpp \
    glm/gtx/closest_point.hpp \
    glm/gtx/color_encoding.hpp \
    glm/gtx/color_space.hpp \
    glm/gtx/color_space_YCoCg.hpp \
    glm/gtx/common.hpp \
    glm/gtx/compatibility.hpp \
    glm/gtx/component_wise.hpp \
    glm/gtx/dual_quaternion.hpp \
    glm/gtx/easing.hpp \
    glm/gtx/euler_angles.hpp \
    glm/gtx/extend.hpp \
    glm/gtx/extended_min_max.hpp \
    glm/gtx/exterior_product.hpp \
    glm/gtx/fast_exponential.hpp \
    glm/gtx/fast_square_root.hpp \
    glm/gtx/fast_trigonometry.hpp \
    glm/gtx/functions.hpp \
    glm/gtx/gradient_paint.hpp \
    glm/gtx/handed_coordinate_space.hpp \
    glm/gtx/hash.hpp \
    glm/gtx/integer.hpp \
    glm/gtx/intersect.hpp \
    glm/gtx/io.hpp \
    glm/gtx/log_base.hpp \
    glm/gtx/matrix_cross_product.hpp \
    glm/gtx/matrix_decompose.hpp \
    glm/gtx/matrix_factorisation.hpp \
    glm/gtx/matrix_interpolation.hpp \
    glm/gtx/matrix_major_storage.hpp \
    glm/gtx/matrix_operation.hpp \
    glm/gtx/matrix_query.hpp \
    glm/gtx/matrix_transform_2d.hpp \
    glm/gtx/mixed_product.hpp \
    glm/gtx/norm.hpp \
    glm/gtx/normal.hpp \
    glm/gtx/normalize_dot.hpp \
    glm/gtx/number_precision.hpp \
    glm/gtx/optimum_pow.hpp \
    glm/gtx/orthonormalize.hpp \
    glm/gtx/perpendicular.hpp \
    glm/gtx/polar_coordinates.hpp \
    glm/gtx/projection.hpp \
    glm/gtx/quaternion.hpp \
    glm/gtx/range.hpp \
    glm/gtx/raw_data.hpp \
    glm/gtx/rotate_normalized_axis.hpp \
    glm/gtx/rotate_vector.hpp \
    glm/gtx/scalar_multiplication.hpp \
    glm/gtx/scalar_relational.hpp \
    glm/gtx/spline.hpp \
    glm/gtx/std_based_type.hpp \
    glm/gtx/string_cast.hpp \
    glm/gtx/texture.hpp \
    glm/gtx/transform.hpp \
    glm/gtx/transform2.hpp \
    glm/gtx/type_aligned.hpp \
    glm/gtx/type_trait.hpp \
    glm/gtx/vec_swizzle.hpp \
    glm/gtx/vector_angle.hpp \
    glm/gtx/vector_query.hpp \
    glm/gtx/wrap.hpp \
    glm/integer.hpp \
    glm/mat2x2.hpp \
    glm/mat2x3.hpp \
    glm/mat2x4.hpp \
    glm/mat3x2.hpp \
    glm/mat3x3.hpp \
    glm/mat3x4.hpp \
    glm/mat4x2.hpp \
    glm/mat4x3.hpp \
    glm/mat4x4.hpp \
    glm/matrix.hpp \
    glm/packing.hpp \
    glm/simd/common.h \
    glm/simd/exponential.h \
    glm/simd/geometric.h \
    glm/simd/integer.h \
    glm/simd/matrix.h \
    glm/simd/packing.h \
    glm/simd/platform.h \
    glm/simd/trigonometric.h \
    glm/simd/vector_relational.h \
    glm/trigonometric.hpp \
    glm/vec2.hpp \
    glm/vec3.hpp \
    glm/vec4.hpp \
    glm/vector_relational.hpp \
    glm_pch.h \
    openglrenderer.h \
    stb_image.h

DISTFILES +=
