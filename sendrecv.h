#pragma once
#include <mpi.h> 

#define REQ_SEND_CODE 1
#define REQ_ACCEPT_CODE 2
#define SEND_RES_CODE 4096
#define SEND_FIELD_CODE 8192 
#define SEND_CALC_TOP_CODE 3
#define SEND_CALC_BOT_CODE 4

inline void SendRow(vector<int>& data, int dest, int tag) 
{
	const int sz = data.size();
	int tmp[sz];
	for (size_t i = 0; i < sz; ++i)
		tmp[i] = data[i];
	MPI_Send(tmp, data.size(), MPI_INT, dest, tag, MPI_COMM_WORLD);
}
inline void SendField(vector<vector<int>>& data, int dest, int tag)
{
	int size = data.size();
	//printf("dest:%d|%d:%d\n",dest, data.size(), data[0].size());
	vector<int> sizes;
	sizes.resize(size);
	for (size_t i = 0; i < size; ++i)
	{
		sizes[i] = data[i].size();
	}  
	MPI_Send(&size, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
	MPI_Send(&sizes[0], sizes.size(), MPI_INT, dest, tag + 1, MPI_COMM_WORLD);
	for (size_t i = 0; i < data.size();++i)
	{
		SendRow(data[i], dest, tag + i + 2);
	}
}
inline void RecvRow(vector<int>& data, int src, int tag)
{
	MPI_Status status;
	const int sz = data.size();
	int tmp[sz];
	MPI_Recv(tmp, sz, MPI_INT, src, tag, MPI_COMM_WORLD, &status);
	for (int i = 0 ; i < sz; ++i)
		data[i] = tmp[i];
}

inline void RecvField(vector<vector<int>>& data, int src, int tag)
{
	MPI_Status status;
	vector<vector<int>> tmp;
	int size;
	MPI_Recv(&size, 1, MPI_INT, src, tag, MPI_COMM_WORLD, &status);
	vector<int> sizes;
	sizes.resize(size);
	MPI_Recv(&sizes[0], size, MPI_INT, src, tag + 1, MPI_COMM_WORLD, &status);
	data.resize(size);

	for (size_t i = 0; i < data.size();++i)
	{
		data[i].resize(sizes[i]);
		RecvRow(data[i], src, tag + i + 2);
	}
	//printf("src:%d|%d:%d\n",src, data.size(), data[0].size());

}

