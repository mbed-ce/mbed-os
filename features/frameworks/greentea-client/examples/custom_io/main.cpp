/*
 * Copyright (c) 2021, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cstdio>
#include "greentea-client/test_env.h"

/* Implementation of test_io.h */

static FILE *input;
static FILE *output;

int greentea_getc()
{
    if (!input) {
        printf("Error: No input file\r\n");
        return EOF;
    }

    return fgetc(input);
}

void greentea_putc(int c)
{
    if (!output) {
        printf("Error: No output file\r\n");
        return;
    }

    int ret = fputc(c, output);
    if (ret == EOF) {
        printf("Error: fputc failed\r\n");
    }
}

void greentea_write_string(const char *str)
{
    if (!output) {
        printf("Error: No output file\r\n");
        return;
    }

    int ret = fputs(str, output);
    if (ret == EOF) {
        printf("Error: fputs failed\r\n");
    }
}

/* Example */

int main()
{
    const char *in_file = "in.txt";
    // read/write (needs write to create file)
    input = fopen(in_file, "w+");
    if (!input) {
        printf("Failed to open %s\r\n", in_file);
        return -1;
    }

    const char *out_file = "out.txt";
    // write only
    output = fopen(out_file, "w");
    if (!output) {
        printf("Failed to open %s\r\n", out_file);
        fclose(input);
        return -1;
    }

    const char *key = "demo_key";
    const int value = 60;
    greentea_send_kv(key, value);

    fclose(input);
    fclose(output);

    return 0;
}
