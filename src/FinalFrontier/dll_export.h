#pragma once

// simplified version of dll export that only handles the windows 
// case. For Linux/Mac case, the FINALFRONTIER_API need not be 
// set to anything.

#ifdef FINALFRONTIER_DLL_EXPORTS
#define FINALFRONTIER_API __declspec(dllexport)
#else
#define FINALFRONTIER_API __declspec(dllimport) 
#endif
