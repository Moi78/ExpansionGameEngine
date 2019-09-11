#include "pch.h"
#include <pybind11/pybind11.h>

#include <iostream>

#include <BulldozerFileManager.h>
#include <BD_Writer.h>

#include <vec3.h>

PYBIND11_MODULE(BulldoPy, m) {
	pybind11::class_<BD_Writer>(m, "BD_Writer")
		.def(pybind11::init<>())
		.def("AppendIndices", &BD_Writer::AppendIndices)
		.def("AppendVertex", &BD_Writer::AppendVertex)
		.def("AppendNormal", &BD_Writer::AppendNormal)
		.def("AppendUVcoord", &BD_Writer::AppendUVcoord)
		
		.def("ToBinary", &BD_Writer::ToBinary);

	pybind11::class_<vec3d>(m, "vec3d")
		.def(pybind11::init<float, float, float>())

		.def("dotProduct", &vec3d::dotProduct)

		.def("getX", &vec3d::getX)
		.def("getY", &vec3d::getY)
		.def("getZ", &vec3d::getZ)

		.def("setAll", &vec3d::setAll)

		.def("setX", &vec3d::setX)
		.def("setY", &vec3d::setY)
		.def("setZ", &vec3d::setZ);
}