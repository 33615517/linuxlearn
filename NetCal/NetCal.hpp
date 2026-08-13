#pragma once

#include "Protocol.hpp"
#include <iostream>

class Cal
{
public:
    Response Execute(Request &req)
    {
        Response resp(0, 0);
        switch (req.Oper())
        {
        case '+':
            resp.SetResult(req.X() + req.Y());
            break;
        case '-':
            resp.SetResult(req.X() - req.Y());
            break;
        case '*':
            resp.SetResult(req.X() * req.Y());
            break;
        case '/':
            if (req.Y() == 0)
            {
                resp.SetCode(1);
            }
            else
            {
                resp.SetResult(req.X() / req.Y());
            }
            break;
        case '%':
            if (req.Y() == 0)
            {
                resp.SetCode(2);
            }
            else
            {
                resp.SetResult(req.X() % req.Y());
            }
            break;
        }
        return resp;
    }

private:
};