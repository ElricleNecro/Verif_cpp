#ifndef VERIF_WRITER_IO_H
#define VERIF_WRITER_IO_H

#include <map>
#include <string>

#include "define.h"
#include "plugins/Plugins.hpp"

namespace io {
	namespace writer {
		class PlugWriter {
			public:
				PlugWriter(const char *fname);
				virtual ~PlugWriter(void);

				virtual void Write(double *data, const int N, const char *fname);
		};

		class Writer {
			public:
				Writer(void);
				~Writer(void);

				void LoadPlugins(const std::string &file_name);
				//New GetInstance(const std::string& func);

			private:
				std::map<std::string, plugins::Plugins> PlugList;
		};

		<template T>
		struct to_write {
			std::string key;
			T* data;
		};

		class HDF5Writer {
			private:
				H5File file;

			public:
				HDF5Writer(const std::string& name);

				H5::DataSet NewDataSet(const std::string& name, const H5::DataType& type);
				Append(const std::string& node, double* tab, const int N);
				Append(const std::string& node, int* tab, const int N);
		};
	}
}

#endif /* end of include guard */
