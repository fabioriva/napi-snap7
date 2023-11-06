#ifndef PROMISE_H
#define PROMISE_H

using namespace Napi;

class ReadAreaPromiseWorker : public AsyncWorker
{
public:
    ReadAreaPromiseWorker(Napi::Env &env, TS7Client *Client, int Area, int DBNumber, int Start, int Amount, int WordLen, int WordSize)
        : AsyncWorker(env),
          Client(Client),
          Area(Area),
          DBNumber(DBNumber),
          Start(Start),
          Amount(Amount),
          WordLen(WordLen),
          WordSize(WordSize),
          deferred(Promise::Deferred::New(env)) {}

    ~ReadAreaPromiseWorker() {}

    void Execute()
    {
        dataLength = Amount * WordSize;
        dataPtr = new uint8_t[dataLength];

        Client->mutex.lock();

        res = Client->ReadArea(Area, DBNumber, Start, Amount, WordLen, dataPtr);

        Client->mutex.unlock();

        if (res != 0)
        {
            AsyncWorker::SetError("Eroor");
        }
    }

    void OnOK()
    {
        deferred.Resolve(Buffer<uint8_t>::New(Env(), dataPtr, dataLength));
    }

    void OnError(Error const &error)
    {
        // deferred.Reject(error.Value());
        deferred.Reject(Number::New(Env(), res));
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

class WriteAreaPromiseWorker : public AsyncWorker
{
public:
    WriteAreaPromiseWorker(Napi::Env &env, TS7Client *Client, int Area, int DBNumber, int Start, int Amount, int WordLen, uint8_t *Buffer)
        : AsyncWorker(env),
          Client(Client),
          Area(Area),
          DBNumber(DBNumber),
          Start(Start),
          Amount(Amount),
          WordLen(WordLen),
          Buffer(Buffer),
          deferred(Promise::Deferred::New(env)) {}

    ~WriteAreaPromiseWorker() {}

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

class DBReadPromiseWorker : public AsyncWorker
{
public:
    DBReadPromiseWorker(Napi::Env &env, TS7Client *Client, int DBNumber, int Start, int WordSize)
        : AsyncWorker(env),
          Client(Client),
          DBNumber(DBNumber),
          Start(Start),
          WordSize(WordSize),
          deferred(Promise::Deferred::New(env)) {}

    ~DBReadPromiseWorker() {}

    void Execute()
    {
        dataLength = 1 * WordSize;
        dataPtr = new uint8_t[dataLength];

        Client->mutex.lock();

        res = Client->DBRead(DBNumber, Start, WordSize, dataPtr);

        Client->mutex.unlock();

        if (res != 0)
        {
            AsyncWorker::SetError("Eroor");
        }
    }

    void OnOK()
    {
        deferred.Resolve(Buffer<uint8_t>::New(Env(), dataPtr, dataLength));
    }

    void OnError(Error const &error)
    {
        // deferred.Reject(error.Value());
        deferred.Reject(Number::New(Env(), res));
    }

    Promise GetPromise() { return deferred.Promise(); }

private:
    TS7Client *Client;
    int DBNumber;
    int Start;
    int WordSize;
    int res;
    uint8_t *dataPtr;
    size_t dataLength;

    Promise::Deferred deferred;
};

class DBWritePromiseWorker : public AsyncWorker
{
public:
    DBWritePromiseWorker(Napi::Env &env, TS7Client *Client, int DBNumber, int Start, int WordSize, uint8_t *Buffer)
        : AsyncWorker(env),
          Client(Client),
          DBNumber(DBNumber),
          Start(Start),
          WordSize(WordSize),
          Buffer(Buffer),
          deferred(Promise::Deferred::New(env)) {}

    ~DBWritePromiseWorker() {}

    void Execute()
    {
        Client->mutex.lock();

        res = Client->DBWrite(DBNumber, Start, WordSize, Buffer);

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
    int DBNumber;
    int Start;
    int WordSize;
    int res;
    uint8_t *Buffer;

    Promise::Deferred deferred;
};
#endif
