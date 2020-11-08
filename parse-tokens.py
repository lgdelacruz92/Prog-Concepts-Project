with open('tokens.txt', 'r') as tokens:
    for line in tokens:
        the_line = line.replace('\n', '')
        rows = the_line.split()
        print('predefined_tokens.insert("{token}");'.format(token=rows[1]))