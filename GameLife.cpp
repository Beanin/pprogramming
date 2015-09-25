#include <pthread.h>
#include <iostream>
#include <vector>
#include <semaphore.h>
#include <random>
#include <string>

using std::vector;

struct TThreadArg {
    vector<vector<int>> *GlobalFieldPtr;
    vector<vector<int>> *SavedField;
    pthread_mutex_t SaveMtx;
    size_t Hight, Width, CornerY, CornerX;
    pthread_barrier_t *ReadBarrier, *WriteBarrier;
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

void* Handle(void* args) {
    TThreadArg ThrdArg = *((TThreadArg*) args);
    delete (TThreadArg*)args;
    size_t IterNumber = 1;
    vector<vector<int>> Field;
    while (!(*end)) {
        IterNumer = 1;
        while (!(*Stop)) {
            GetUpdate(*GlobalFieldPtr, &Field, CornerY, CornerX);
            for (size_t y = 1; y < Field.size() - 1; ++y) {
                for (size_t x = 1; x < Field[0].size(); ++x) {
                    if (NeighboursCount(Field ,x ,y) == 3 || NeighboursCount(Field, x, y) == 2 && Field[y][x]) 
                        Field[y][x] = 1;
                    else
                        Field[y][x] = 0;
                }
            }
        //CV Read
            pthread_barrier_wait(ReadBarrier);
            SendRes(Field, GlobalFieldPtr, CornerY, CornerX);
        //CV Send
            if (!pthread_barrier_wait() == PTHREAD_BARRIER_SERIAL_THREAD) {
                pthread_mutex_lock(SaveMtx);
                *SavedField = *GlobalFieldPtr;
                pthread_mutex_unlock(SaveMtx);
            }
        }
    }
}
vector<vector<int>> GetField() {
    vector<vector<int>> Res;
    return Res;
}
vector<vector<int>> GenerateField() {
    vector<vector<int>> Res;
    return Res;
}

void master() {
    size_t ThreadsCount;
    vector<vector<int>> PlayFieldEvenIteration = GetField();
    Height = PlayFieldEvenIteration.size();
    Width = PlayFieldEvenIteration[0].size();
    vector<vector<int>> PlayFieldOddIteration(Heigth, vector<int>(Width));
    vector<pthread_t> Threads(ThreadsCount);
    pthread_cond_t UpdateCV;
    bool Updated;
    pthread_barrier_t ReadBarrier, WriteBarrier;
    pthread_barrier_init(&ReadBarrier, nullptr, ThreadsCount);
    pthread_barrier_init(&WriteBarrier, nullptr, ThreadsCount);
    //Need partition
    for (size_t ThrdNum = 0; ThrdNum < ThreadsCount; ++ThrdNum) {
        size_t CellsForThread = Heigth * Width / ThreadsCount;
        size_t StartingCell = ThrdNum * CellsForThread;
        pthread_create(&Threads[ThrdNum], NULL, Handle,
                       (void*)//arg    }
    for (size_t ThrdNum = 0; ThrdNum < ThreadsCount; ++ThrdNum) {
        pthread_join(Threads[ThrdNum], NULL);
    }
}

int main() {

}