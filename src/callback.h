#ifndef CALLBACK_H
#define CALLBACK_H

using namespace Napi;

class ReadCallbackWorker : public AsyncWorker
{
public:
    ReadCallbackWorker(Function &callback, TS7Client *Client, int Area, int DBNumber, int Start, int Amount, int WordLen, int WordSize)
        : AsyncWorker(callback),
          Client(Client),
          Area(Area),
          DBNumber(DBNumber),
          Start(Start),
          Amount(Amount),
          WordLen(WordLen),
          WordSize(WordSize) {}

    ~ReadCallbackWorker() {}

    void Execute()
    {
        dataLength = Amount * WordSize;
        dataPtr = new uint8_t[dataLength];

        Client->mutex.lock();

        res = Client->ReadArea(Area, DBNumber, Start, Amount, WordLen, dataPtr);

        Client->mutex.unlock();
    }

    void OnOK()
    {
        // HandleScope scope(Env());
        if (res == 0)
        {
            Callback().Call({Env().Undefined(), Buffer<uint8_t>::New(Env(), dataPtr, dataLength)});
        }
        else
        {
            Callback().Call({Number::New(Env(), res)});
        }
    }

private:
    TS7Client *Client;
    int Area;
    int DBNumber;
    int Start;
    int Amount;
    int WordLen;
    int WordSize;
    int res;
    uint8_t *dataPtr;
    size_t dataLength;
};

class WriteCallbackWorker : public AsyncWorker
{
public:
    WriteCallbackWorker(Function &callback, TS7Client *Client, int Area, int DBNumber, int Start, int Amount, int WordLen, uint8_t *Buffer)
        : AsyncWorker(callback),
          Client(Client),
          Area(Area),
          DBNumber(DBNumber),
          Start(Start),
          Amount(Amount),
          WordLen(WordLen),
          Buffer(Buffer) {}

    ~WriteCallbackWorker() {}

    void Execute()
    {
        Client->mutex.lock();

        res = Client->WriteArea(Area, DBNumber, Start, Amount, WordLen, Buffer);

        Client->mutex.unlock();
    }

    void OnOK()
    {
        Callback().Call({Number::New(Env(), res)});
    }

private:
    TS7Client *Client;
    int Area;
    int DBNumber;
    int Start;
    int Amount;
    int WordLen;
    int WordSize;
    int res;
    uint8_t *Buffer;
};

#endif