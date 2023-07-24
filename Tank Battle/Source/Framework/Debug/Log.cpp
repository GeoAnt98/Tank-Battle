#include "Log.h"
#include <GameDev2D.h>
#include <stdio.h>
#include <string>
#include <Windows.h>


namespace GameDev2D
{
    bool Log::s_IsInitialized = false;

    void Log::Init()
    {
#if LOG_TO_FILE
        if (s_IsInitialized == false)
        {
            //Get the Path for the Working Directory and append the Log filename to it
            string path = string(Services::GetApplication()->GetWorkingDirectory());
            path += LOG_FILE;

            //Open the log file, wiping any existing data in the file
            FILE* logFile;
            logFile = fopen(path.c_str(), "w");

            //Make sure the file opened, if it did close the file
            if (logFile != NULL)
            {
                s_IsInitialized = true;
                fprintf(logFile, "GameDev2D initiated\n\n");
                fclose(logFile);
            }
        }
#endif
    }

    void Log::Message(const char* aMessage, ...)
    {
#if DEBUG || _DEBUG
        va_list arguments;
        va_start(arguments, aMessage);
        Log::Output(false, "", Verbosity_Debug, nullptr, aMessage, arguments);
        va_end(arguments);
#endif
    }

    void Log::Message(const char* aLabel, const char* aMessage, ...)
    {
#if DEBUG || _DEBUG
        va_list arguments;
        va_start(arguments, aMessage);
        Log::Output(false, aLabel, Verbosity_Debug, nullptr, aMessage, arguments);
        va_end(arguments);
#endif
    }

    void Log::Message(const char* aLabel, Verbosity aVerbosity, const char* aMessage, ...)
    {
#if DEBUG || _DEBUG
        va_list arguments;
        va_start(arguments, aMessage);
        Log::Output(false, aLabel, aVerbosity, nullptr, aMessage, arguments);
        va_end(arguments);
#endif
    }

    void Log::Message(void* aAddress, const char* aLabel, Verbosity aVerbosity, const char* aMessage, ...)
    {
#if DEBUG || _DEBUG
        va_list arguments;
        va_start(arguments, aMessage);
        Log::Output(false, aLabel, aVerbosity, aAddress, aMessage, arguments);
        va_end(arguments);
#endif
    }

    void Log::Error(void* aAddress, const char* aLabel, bool aThrowException, Verbosity aVerbosity, const char* aMessage, ...)
    {
#if DEBUG || _DEBUG
        va_list arguments;
        va_start(arguments, aMessage);
        Log::Output(true, aLabel, aVerbosity, aAddress, aMessage, arguments);
        va_end(arguments);


        
#if THROW_EXCEPTION_ON_ERROR
        //Should an exception be thrown?
        if (aThrowException == true)
        {
            char outputBuffer[1024];
            vsnprintf(outputBuffer, 1024, aMessage, arguments);

            throw std::runtime_error(outputBuffer);
        }

        //End the variable argument list
        va_end(arguments);
#endif
#endif
    }

    void Log::Output(bool aError, const char* aLabel, Verbosity aVerbosity, void* aAddress, const char* aOutput, va_list aArgumentsList)
    {
#if DEBUG || _DEBUG
        //Is this an error?
        if (aError == true)
        {
            char labelBuffer[140];
            sprintf_s(labelBuffer, 128, "[%s][%p] *** ERROR *** ", aLabel, aAddress);
            OutputDebugStringA(labelBuffer);

            ////Output to the window's 'Output' window
            char outputBuffer[1024];
            vsnprintf(outputBuffer, 1024, aOutput, aArgumentsList);
            OutputDebugStringA(outputBuffer);
            OutputDebugStringA("\n");
        }
        else
        {
            if ((aVerbosity & (LOG_VERBOSITY_MASK)) != 0)
            {
                if (aAddress == nullptr)
                {
                    if (aLabel != "")
                    {
                        char labelBuffer[128];
                        sprintf_s(labelBuffer, 128, "[%s] ", aLabel);
                        OutputDebugStringA(labelBuffer);
                    }
                }
                else
                {
                    if (aLabel != "")
                    {
                        char labelBuffer[128];
                        sprintf_s(labelBuffer, 128, "[%s][%p] ", aLabel, aAddress);
                        OutputDebugStringA(labelBuffer);
                    }
                    else
                    {
                        char labelBuffer[128];
                        sprintf_s(labelBuffer, 64, "[%p] ", aAddress);
                        OutputDebugStringA(labelBuffer);
                    }
                }

                // Output to the window's 'Output' window
                //char outputBuffer[1024];
                //vsnprintf(outputBuffer, 1024, aOutput, aArgumentsList);
                //OutputDebugStringA(outputBuffer);
                //OutputDebugStringA("\n");
            }


#if LOG_TO_FILE
            if (s_IsInitialized == true)
            {
                //Get the path for the log file
                string path = string(Services::GetApplication()->GetWorkingDirectory());
                path += LOG_FILE;

                //Open the file at the path, if the assert is hit it means the log file couldn't be opened
                FILE* logFile;
                logFile = fopen(path.c_str(), "a+");
                assert(logFile != nullptr);

                //Write the log to the file
                if (logFile != NULL)
                {
                    fprintf(logFile, "[%p][%s] - ", aAddress, aLabel);
                    vfprintf(logFile, aOutput, aArgumentsList);
                    fprintf(logFile, "\n");
                    fclose(logFile);
                }
            }
#endif
        }
#endif
    }
}