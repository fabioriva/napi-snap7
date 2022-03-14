#ifndef PROMISE_H
#define PROMISE_H

using namespace Napi;

class ReadPromiseWorker : public AsyncWorker
{
public:
    ReadPromiseWorker(Napi::Env &env, TS7Client *Client, int Area, int DBNumber, int Start, int Amount, int WordLen, int WordSize)
        : AsyncWorker(env),
          Client(Client),
          Area(Area),
          DBNumber(DBNumber),
          Start(Start),
          Amount(Amount),
          WordLen(WordLen),
          WordSize(WordSize),
          deferred(Promise::Deferred::New(env)) {}

    ~ReadPromiseWorker() {}

    void Execute()
    {
        dataLength = Amount * WordSize;
        dataPtr = new uint8_t[dataLength];

        Client->mutex.lock();

        res = Client->ReadArea(Area, DBNumber, Start, Amount, WordLen, dataPtr);

        Client->mutex.unlock();

        if (res != 0)
        {
            AsyncWorker::SetError("Error!");
        }
    }

    void OnOK()
    {
        if (res == 0)
        {
            deferred.Resolve(Buffer<uint8_t>::New(Env(), dataPtr, dataLength));
        }
        else
        {
            deferred.Resolve(Number::New(Env(), res));
        }
    }

    void OnError(Error const &error)
    {
        deferred.Reject(error.Value());
    }

    Promise GetPromise() { return deferred.Promise(); }

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

    Promise::Deferred deferred;
};

class WritePromiseWorker : public AsyncWorker
{
public:
    WritePromiseWorker(Napi::Env &env, TS7Client *Client, int Area, int DBNumber, int Start, int Amount, int WordLen, uint8_t *Buffer)
        : AsyncWorker(env),
          Client(Client),
          Area(Area),
          DBNumber(DBNumber),
          Start(Start),
          Amount(Amount),
          WordLen(WordLen),
          Buffer(Buffer),
          deferred(Promise::Deferred::New(env)) {}

    ~WritePromiseWorker() {}

    void Execute()
    {
        Client->mutex.lock();

        res = Client->WriteArea(Area, DBNumber, Start, Amount, WordLen, Buffer);

        Client->mutex.unlock();

        if (res != 0)
        {
            AsyncWorker::SetError("Error!");
        }
    }

    void OnOK()
    {
        deferred.Resolve(Number::New(Env(), res));
    }

    void OnError(Error const &error)
    {
        deferred.Reject(error.Value());
    }

    Promise GetPromise() { return deferred.Promise(); }

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

    Promise::Deferred deferred;
};

#endif
