CXX=g++
DEP_DIR=$(PWD)/deps
CUDA_VERSION?=11.1
ROOT_DIR?="../../../" # path to TRTorch directory (including TRTorch)
INCLUDE_DIRS=-I$(DEP_DIR)/libtorch/include  -I$(ROOT_DIR) -I$(DEP_DIR)/trtorch/include -I$(DEP_DIR)/libtorch/include/torch/csrc/api/include/ -I/usr/local/cuda-$(CUDA_VERSION)/include -I$(DEP_DIR)/tensorrt/include
LIB_DIRS=-L$(DEP_DIR)/trtorch/lib -L$(DEP_DIR)/libtorch/lib -L/usr/local/cuda-$(CUDA_VERSION)/lib64
LIBS=-ltrtorch -ltorch -ltorch_cuda -ltorch_cpu -ltorch_global_deps -lbackend_with_compiler -lc10 -lc10_cuda -lpthread -lcudart
SRCS=../datasets/cifar10.cpp ../benchmark/benchmark.cpp main.cpp

TARGET=ptq

$(TARGET):
	$(CXX) $(SRCS) $(INCLUDE_DIRS) $(LIB_DIRS) $(LIBS) -o $(TARGET)

clean:
	$(RM) $(TARGET)
