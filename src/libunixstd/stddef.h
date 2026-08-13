
/*
    stddef header
*/

#pragma once

#define offsetof(type, member) ((unsigned long) &((type*)0)->member)
