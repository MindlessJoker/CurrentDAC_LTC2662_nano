/*-
 * Copyright (c) 2012-2013 Jan Breuer,
 *
 * All Rights Reserved
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file   lexer.h
 * @date   Thu Mar 21 15:00:58 UTC 2013
 * 
 * @brief  SCPI Lexer
 * 
 * 
 */

#ifndef SCPI_LEXER_H
#define SCPI_LEXER_H

#include "scpi/types.h"
#include "utils_private.h"

#ifdef  __cplusplus
extern "C" {
#endif

    int scpiLex_IsEos(lex_state_t * state) LOCAL;
    int scpiLex_WhiteSpace(lex_state_t * state, scpi_token_t * token) LOCAL;
    int scpiLex_ProgramHeader(lex_state_t * state, scpi_token_t * token) LOCAL;
    int scpiLex_CharacterProgramData(lex_state_t * state, scpi_token_t * token) LOCAL;
    int scpiLex_DecimalNumericProgramData(lex_state_t * state, scpi_token_t * token) LOCAL;
    int scpiLex_SuffixProgramData(lex_state_t * state, scpi_token_t * token) LOCAL;
    int scpiLex_NondecimalNumericData(lex_state_t * state, scpi_token_t * token) LOCAL;
    int scpiLex_StringProgramData(lex_state_t * state, scpi_token_t * token) LOCAL;
    int scpiLex_ArbitraryBlockProgramData(lex_state_t * state, scpi_token_t * token) LOCAL;
    int scpiLex_ProgramExpression(lex_state_t * state, scpi_token_t * token) LOCAL;
    int scpiLex_Comma(lex_state_t * state, scpi_token_t * token) LOCAL;
    int scpiLex_Semicolon(lex_state_t * state, scpi_token_t * token) LOCAL;
    int scpiLex_NewLine(lex_state_t * state, scpi_token_t * token) LOCAL;

#ifdef  __cplusplus
}
#endif

#endif  /* SCPI_LEXER_H */
