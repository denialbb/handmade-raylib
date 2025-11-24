#include "managers/config.hpp"
#include "raylib.h"
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>

GameConfig config = {0};
// NOTE: This is a bit of a hack, we should probably use a more robust
// solution in the future.
// We are using a static buffer to store the trimmed string, this is not
// thread-safe.
// NOTE: Caller must free the returned char* using RL_FREE
char *trim(const char *str) {
    const char *start = str;
    while (*start && std::isspace(*start)) {
        start++;
    }

    const char *end = str + std::strlen(str) - 1;
    while (end >= start && std::isspace(*end)) {
        end--;
    }

    int length = (end >= start) ? (end - start + 1) : 0;

    // Use RL_MALLOC instead of new char[]
    char *buffer = (char *)RL_MALLOC(length + 1);
    if (buffer == NULL) { // Handle allocation failure
        return NULL;
    }

    if (length > 0) {
        std::strncpy(buffer, start, length);
    }
    buffer[length] = '\0';

    return buffer;
}

void LoadConfig(const char *fileName) {
    char *fileContent = LoadFileText(fileName);
    if (fileContent == NULL) {
        printf("Failed to load config file: %s\n", fileName);
        return;
    }

    int lineCount;
    char **lines = TextSplit(fileContent, '\n', &lineCount); // Raylib function

    for (int i = 0; i < lineCount; i++) {
        char *line = lines[i];
        char *trimmedLine =
            trim(line); // trim returns RL_MALLOCed char*, needs RL_FREE

        if (trimmedLine == NULL) { // Handle trim allocation failure
            RL_FREE(lines);
            UnloadFileText(fileContent);
            return;
        }

        if (trimmedLine[0] == '[' || trimmedLine[0] == '\0' ||
            trimmedLine[0] == ';') {
            RL_FREE(trimmedLine); // Free memory from trim
            continue;
        }

        char *equalSign = std::strchr(trimmedLine, '=');
        if (equalSign == NULL) {
            RL_FREE(trimmedLine); // Free memory from trim
            continue;
        }

        // Extract key
        char *keyBuffer =
            (char *)RL_MALLOC(equalSign - trimmedLine + 1); // Use RL_MALLOC
        if (keyBuffer == NULL) { // Handle allocation failure
            RL_FREE(trimmedLine);
            RL_FREE(lines);
            UnloadFileText(fileContent);
            return;
        }
        std::strncpy(keyBuffer, trimmedLine, equalSign - trimmedLine);
        keyBuffer[equalSign - trimmedLine] = '\0';
        char *trimmedKey =
            trim(keyBuffer); // trim returns RL_MALLOCed char*, needs RL_FREE
        RL_FREE(keyBuffer);  // Free temp buffer using RL_FREE

        if (trimmedKey == NULL) { // Handle trim allocation failure
            RL_FREE(trimmedLine);
            RL_FREE(lines);
            UnloadFileText(fileContent);
            return;
        }

        // Extract value
        char *value = trim(equalSign +
                           1); // trim returns RL_MALLOCed char*, needs RL_FREE

        if (value == NULL) { // Handle trim allocation failure
            RL_FREE(trimmedLine);
            RL_FREE(trimmedKey);
            RL_FREE(lines);
            UnloadFileText(fileContent);
            return;
        }

        // (Commented out printf line)

        if (std::strcmp(trimmedKey, "ost") == 0) {
            // Need to copy value, as config.ost_path will be RL_FREE'd by
            // UnloadConfig and `value` itself needs to be RL_FREE'd too.
            config.ost_path = (char *)RL_MALLOC(std::strlen(value) + 1);
            if (config.ost_path ==
                NULL) { /* Handle error, or re-enable printf for debug */
            }
            std::strcpy(config.ost_path, value);
            RL_FREE(value); // Free `value` after copying
        } else if (std::strcmp(trimmedKey, "sheet") == 0) {
            config.spritesheet_path = (char *)RL_MALLOC(std::strlen(value) + 1);
            if (config.spritesheet_path ==
                NULL) { /* Handle error, or re-enable printf for debug */
            }
            std::strcpy(config.spritesheet_path, value);
            RL_FREE(value); // Free `value` after copying
        } else if (std::strcmp(trimmedKey, "sprite_id_x") == 0) {
            config.player_sprite_id_x = TextToInteger(value);
            RL_FREE(value); // Free value after use for integers
        } else if (std::strcmp(trimmedKey, "sprite_id_y") == 0) {
            config.player_sprite_id_y = TextToInteger(value);
            RL_FREE(value);
        } else if (std::strcmp(trimmedKey, "width") == 0) {
            config.player_width = TextToInteger(value);
            RL_FREE(value);
        } else if (std::strcmp(trimmedKey, "height") == 0) {
            config.player_height = TextToInteger(value);
            RL_FREE(value);
        } else {
            // If key not recognized, free value
            RL_FREE(value);
        }

        RL_FREE(trimmedLine); // Free memory for trimmedLine after all uses
        RL_FREE(trimmedKey);  // Free memory for trimmedKey after all uses
    }

    RL_FREE(lines); // Free memory allocated by TextSplit
    UnloadFileText(fileContent);
}

void UnloadConfig() {
    if (config.ost_path) {
        RL_FREE(config.ost_path); // Use RL_FREE
        config.ost_path = nullptr;
    }
    if (config.spritesheet_path) {
        RL_FREE(config.spritesheet_path); // Use RL_FREE
        config.spritesheet_path = nullptr;
    }
}
