#include <pthread.h>
#include <iostream>
#include <vector>
#include <random>
#include <string>


using std::vector;

struct TThreadArg {
    vector<vector<int>> *GlobalFieldPtr;
    vector<vector<int>> *GlobalStableFieldPtr;
    pthread_mutex_t *UpdateStableMtx;
    pthread_cond_t UpdateCV;
    bool *Printing, *Updating;
    size_t Hight, Width, CornerY, CornerX;
    pthread_barrier_t *UpdateBarrier, *UpdateStableBarrier;
    bool *Stop, *End;
};


void GetUpdate(const vector<vector<int>> &Src, vector<vector<int>> *Dest, size_t CornerY, size_t CornerX) {
    for (size_t i = 1; i < Dest->size() - 1; ++i) {
        (*Dest)[i][0] = Src[i + CornerY - 1][(CornerX + Src[0].size() - 1) % Src[0].size()]
        (*Dest)[i][Dest->size() - 1] = Src[i + CornerY - 1][(CornerX + (*Dest)[0].size() - 2) % Src[0].size()];
    }
    for (size_t i = 1; i < (*Dest)[0].size() - 1); ++i) {
        (*Dest)[0][i] = Src[(CornerY + Src.size()- 1) % Src.size()][CornerX + i - 1];
        (*Dest)[(*Dest)[0].size() - 1][i] = Src[(CornerY + Dest->size() - 2) % Src.size()][CornerX + i - 1];
    }
}

size_t NeighboursCount(const vector<vector<int>> &Field, size_t x, size_t y) {
    size_t Cnt = 0;
    for (size_t i = -1; i < 2; ++i) {
        for (size_t j = -1; j < 2; ++j) {
            if (i*i + j*j > 0)
                Cnt+= Field[i+x][j+y];
        }
    }
    return NeighboursCount;
}

void SendField(const vector<vector<int>> &Src) {
    std::cout << "qq" << std::endl;
}

void PrintStable(const vector<vector<int>> &Src, pthread_mutex_t *UpdateMtx, pthread_cond_t *UpdateCV, const bool &Updating) {
    pthread_mutex_lock(UpdateMtx);
    if (Updating) {
        pthread_cond_wait(UpdateCV, UpdateMtx);
    }
    SendField(Src);
    pthread_mutex_unlock(UpdateMtx);
}

void* Handle(void* args) {
    TThreadArg ThrdArg = *((TThreadArg*) args);
    delete (TThreadArg*)args;
    size_t IterNumber = 1;
    vector<vector<int>> Field;
    while (!(*end)) {
        IterNumer = 1;
        while (!(*Stop)) {
            GetUpdate(*GlobalStableFieldPtr, &Field, CornerY, CornerX);
            for (size_t y = 1; y < Field.size() - 1; ++y) {
                for (size_t x = 1; x < Field[0].size(); ++x) {
                    if (NeighboursCount(Field ,x ,y) == 3 || NeighboursCount(Field, x, y) == 2 && Field[y][x]) 
                        Field[y][x] = 1;
                    else
                        Field[y][x] = 0;
                }
            }
            SendRes(Field, GlobalFieldPtr, CornerY, CornerX);
            pthread_barrier_wait(UpdateBarrier);
            pthread_mutex_lock(UpdateStableMtx);
            Updating = true;
            pthread_mutex_unlock(UpdateStableMtx);
            SendRes(Field, GlobalStableFieldPtr, CornerY, CornerX);
            if (pthread_barrier_wait(UpdateStableBarrier) == PTHREAD_BARRIER_SERIAL_THREAD) {
                pthread_mutex_lock(UpdateStableMtx);
                *Updating = false;
                pthread_cond_signal(UpdateCV, UpdateStableMtx);
                pthread_mutex_unlock(UpdateStableMtx);
            }
        }
    }
}




vector<vector<int>> GetFieldFromFile() {
    vector<vector<int>> Res;
    return Res;
}

vector<vector<int>> GetRandomField() {
    size_t 
}

vector<vector<int>> GenerateField() {
    vector<vector<int>> Res;
    return Res;
}

void GetData(size_t &ThreadsCount, size_t &Height, size_t &Width, vector<vector<int>> &Field) {
#ifdef CVS
    Field = GetFieldFromFile();
#ifndef
    Field = GetRandomField();
#endif
    Height = Field.size();
    Width = Field[0].size();
}

void master() {
    size_t ThreadsCount;
    vector<vector<int>> FieldTemporary;
    vector<vector<int>> FieldStable;
    size_t Height;
    size_t Width;
    GetData(ThreadsCount, Height, Width, FieldStable);
    vector<pthread_t> Threads(ThreadsCount);
    pthread_barrier_t UpdateBarrier, UpdateStableBarrierBarrier;
    pthread_barrier_init(&UpdateBarrier, nullptr, ThreadsCount);
    pthread_barrier_init(&UpdateStableBarrier, nullptr, ThreadsCount);
    std::string Request;
    bool Started = False;
    while (std::cin >> Request) {
    //Need partition
        if (Request == "START") {
          
            for (size_t ThrdNum = 0; ThrdNum < ThreadsCount; ++ThrdNum) {
                size_t CellsForThread = Heigth * Width / ThreadsCount;
                size_t StartingCell = ThrdNum * CellsForThread;
                pthread_create(&Threads[ThrdNum], NULL, Handle, (void*)//arg    }
            for (size_t ThrdNum = 0; ThrdNum < ThreadsCount; ++ThrdNum) {
                pthread_join(Threads[ThrdNum], NULL);
            }
        }
    }
}

int main() {

}