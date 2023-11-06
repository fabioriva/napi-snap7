#ifndef CALLBACK_H
#define CALLBACK_H

using namespace Napi;

class ReadAreaCallbackWorker : public AsyncWorker
{
public:
    ReadAreaCallbackWorker(Function &callback, TS7Client *Client, int Area, int DBNumber, int Start, int Amount, int WordLen, int WordSize)
        : AsyncWorker(callback),
          Client(Client),
          Area(Area),
          DBNumber(DBNumber),
          Start(Start),
          Amount(Amount),
          WordLen(WordLen),
          WordSize(WordSize) {}

    ~ReadAreaCallbackWorker() {}

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

class WriteAreaCallbackWorker : public AsyncWorker
{
public:
    WriteAreaCallbackWorker(Function &callback, TS7Client *Client, int Area, int DBNumber, int Start, int Amount, int WordLen, uint8_t *Buffer)
        : AsyncWorker(callback),
          Client(Client),
          Area(Area),
          DBNumber(DBNumber),
          Start(Start),
          Amount(Amount),
          WordLen(WordLen),
          Buffer(Buffer) {}

    ~WriteAreaCallbackWorker() {}

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

class DBReadCallbackWorker : public AsyncWorker
{
public:
    DBReadCallbackWorker(Function &callback, TS7Client *Client, int DBNumber, int Start, int WordSize)
        : AsyncWorker(callback),
          Client(Client),
          DBNumber(DBNumber),
          Start(Start),
          WordSize(WordSize) {}

    ~DBReadCallbackWorker() {}

    void Execute()
    {
        dataLength = 1 * WordSize;
        dataPtr = new uint8_t[dataLength];

        Client->mutex.lock();

        res = Client->DBRead(DBNumber, Start, WordSize, dataPtr);

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
    int DBNumber;
    int Start;
    int WordSize;
    int res;
    uint8_t *dataPtr;
    size_t dataLength;
};

class DBWriteCallbackWorker : public AsyncWorker
{
public:
    DBWriteCallbackWorker(Function &callback, TS7Client *Client, int DBNumber, int Start, int WordSize, uint8_t *Buffer)
        : AsyncWorker(callback),
          Client(Client),
          DBNumber(DBNumber),
          Start(Start),
          WordSize(WordSize),
          Buffer(Buffer) {}

    ~DBWriteCallbackWorker() {}

    void Execute()
    {
        Client->mutex.lock();

        res = Client->DBWrite(DBNumber, Start, WordSize, Buffer);

        Client->mutex.unlock();
    }

    void OnOK()
    {
        Callback().Call({Number::New(Env(), res)});
    }

private:
    TS7Client *Client;
    int DBNumber;
    int Start;
    int WordSize;
    int res;
    uint8_t *Buffer;
};
#endif