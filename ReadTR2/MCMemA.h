/*
 ************************************************************************************
 *
 *      File:           MCMemA.h
 *      Class:          MCMemA
 *
 ************************************************************************************
 *      Description:
 *
 ************************************************************************************
 *      Notes:
 *
 ************************************************************************************
 *
 *      E/Mail: ian.macallan@gmail.com
 *      (c) 2003 - 2023 / Macallan
 *      
 ************************************************************************************
 *
 */
#pragma once

//
//====================================================================================
//
//====================================================================================
class MCMemA
{
    public :
        int len;
        char *ptr;

    public:
        MCMemA(void);
        MCMemA( int sizeInBytes );
        MCMemA ( const char *pText, int sizeInBytes  );
        virtual char *AllocateA( int sizeInBytes );
        virtual void fromPointer ( char *pText, int sizeInBytes );
        virtual void Free();
        virtual ~MCMemA (void);
};

