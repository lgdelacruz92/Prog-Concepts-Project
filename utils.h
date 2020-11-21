bool isAlphabet(char c) {
    return (65 <= c && c <= 90) || (97 <= c && c <= 122);
}

bool isDigit(char c) {
    return 0 <= (c - '0') && (c - '0') <= 9;
}

bool isIdentifierCharacter(char c) {
    return isAlphabet(c) || c == '_' || isDigit(c);
}

bool isIdentifierStart(char c) {
    return isAlphabet(c) || c == '_';
}

bool isWhiteSpace(char c) {
    return c == ' ' || c == '\n' || c == '\t';
}
