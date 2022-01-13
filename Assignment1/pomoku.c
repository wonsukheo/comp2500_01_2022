#include "pomoku.h"

const size_t BOARD_MAX = 20;
const size_t BOARD_MIN = 10;
const size_t BOARD_DEFAULT = 15;

int g_board[20][20];                      /* const array = const pointer? */
size_t g_board_row = BOARD_DEFAULT;       /* can be declared here? */ 
size_t g_board_col = BOARD_DEFAULT;       /* or just init? */

size_t g_black_score = 0;
size_t g_white_score = 0;

void init_game(void)
{
    size_t i;
    size_t j;

    g_board_row = BOARD_DEFAULT;
    g_board_col = BOARD_DEFAULT;
    g_black_score = 0;
    g_white_score = 0;

    for (i = 0; i < BOARD_MAX; ++i) {
        for (j = 0; j < BOARD_MAX; ++j) {
            if (i < BOARD_DEFAULT && j < BOARD_DEFAULT) {
                g_board[i][j] = 1;         /* 1 = valid board */
            } else {                       
                g_board[i][j] = 0;         /* 0 = invalid board */ 
            }
        }
    }
}

size_t get_row_count(void)
{
    return g_board_row;
}

size_t get_column_count(void)
{
    return g_board_col;
}

int get_score(const color_t color)
{
    switch (color) {
        case 'B': 
            return g_black_score;
        case 'W':
            return g_white_score;
        default:
            return -1;
    }
}

int get_color(const size_t row, const size_t col)
{
    if (row >= g_board_row || col >= g_board_col) {
        return -1;
    }
    
    if (g_board[row][col] == 'B') {
        return 0;
    } else if (g_board[row][col] == 'W') {
        return 1;
    } else {
        return -1;
    }
}

int is_placeable(const size_t row, const size_t col)
{
    if (row >= g_board_row || col >= g_board_col) {
        return FALSE;
    }

    return (g_board[row][col] == 1) ? TRUE : FALSE;
}

int place_stone(const color_t color, const size_t row, const size_t col)
{
    if (is_placeable(row, col)) {
        switch (color) {
            case 'B': 
                g_board[row][col] = 'B';

                update_score(color, row, col);

                return TRUE;
            case 'W':
                g_board[row][col] = 'W';
  
                update_score(color, row, col);

                return TRUE;
            default:
                return FALSE;
        }
    }
        
    return FALSE;
}

/* special moves */
int insert_row(const color_t color, const size_t row)
{
    size_t* score;
    size_t i;
    size_t j;

    switch (color) {
        case 'B': 
            score = &g_black_score;
            break;
        case 'W':
            score = &g_white_score;
            break;
        default:
            return FALSE;
    }
    
    if (g_board_row == BOARD_MAX || row > g_board_row || *score < 3) {      /* enum elements can have same value */ 
        return FALSE;
    }
   
    for (i = g_board_row; i > row; --i) {
        for (j = 0; j < g_board_col; ++j) {
            g_board[i][j] = g_board[i - 1][j];
        }
    }

    for (i = 0; i < g_board_col; ++i) {
        g_board[row][i] = 1;
    }  

    g_board_row++;    
    *score -= 3;
    return TRUE;
}

int insert_column(const color_t color, const size_t col)
{
    size_t* score;
    size_t i;
    size_t j;

    switch (color) {
        case 'B': 
            score = &g_black_score;
            break;
        case 'W':
            score = &g_white_score;
            break;
        default:
            return FALSE;
    }
    
    if (g_board_col == BOARD_MAX || col > g_board_col || *score < 3) {
        return FALSE;
    }
   
    for (i = g_board_col; i > col; --i) {
        for (j = 0; j < g_board_row; ++j) {
            g_board[j][i] = g_board[j][i - 1];
        }
    }

    for (i = 0; i < g_board_row; ++i) {
        g_board[i][col] = 1;
    }  

    g_board_col++;    
    *score -= 3;
    return TRUE;
}

int remove_row(const color_t color, const size_t row)
{
    size_t* score;
    size_t i;
    size_t j;

    switch (color) {
        case 'B': 
            score = &g_black_score;
            break;
        case 'W':
            score = &g_white_score;
            break;
        default:
            return FALSE;
    }
    
    if (g_board_row == BOARD_MIN || *score < 3) {
        return FALSE;
    }

    for (i = row; i < g_board_row; ++i) {
        for (j = 0; j < g_board_col; ++j) {
            if (i == g_board_row - 1) {
                g_board[i][j] = 0;
            } else {
                g_board[i][j] = g_board[i + 1][j];
            }
        }
    }

    g_board_row--;    
    *score -= 3;
    return TRUE;
}

int remove_column(const color_t color, const size_t col)
{
    size_t* score;
    size_t i;
    size_t j;

    switch (color) {
        case 'B': 
            score = &g_black_score;
            break;
        case 'W':
            score = &g_white_score;
            break;
        default:
            return FALSE;
    }
    
    if (g_board_col == BOARD_MIN || *score < 3) {
        return FALSE;
    }

    for (i = col; i < g_board_col; ++i) {
        for (j = 0; j < g_board_row; ++j) {
            if (i == g_board_col - 1) {
                g_board[j][i] = 0;
            } else {
                g_board[j][i] = g_board[j][i + 1];
            }
        }
    }

    g_board_col--;    
    *score -= 3;
    return TRUE;
}

int swap_rows(const color_t color, const size_t row0, const size_t row1)
{
    size_t* score;
    int temp;
    size_t i;

    switch (color) {
        case 'B': 
            score = &g_black_score;
            break;
        case 'W':
            score = &g_white_score;
            break;
        default:
            return FALSE;
    }
    
    if (row0 >= g_board_row || row1 >= g_board_row || *score < 2) {
        return FALSE;
    }

    for (i = 0; i < g_board_col; ++i) {
        temp = g_board[row0][i];
        g_board[row0][i] = g_board[row1][i];
        g_board[row1][i] = temp;
    }

    *score -= 2;
    return TRUE;
}

int swap_columns(const color_t color, const size_t col0, const size_t col1)
{
    size_t* score;
    int temp;
    size_t i;

    switch (color) {
        case 'B': 
            score = &g_black_score;
            break;
        case 'W':
            score = &g_white_score;
            break;
        default:
            return FALSE;
    }
    
    if (col0 >= g_board_col || col1 >= g_board_col || *score < 2) {
        return FALSE;
    }

    for (i = 0; i < g_board_row; ++i) {
        temp = g_board[i][col0];
        g_board[i][col0] = g_board[i][col1];
        g_board[i][col1] = temp;
    }

    *score -= 2;
    return TRUE;
}

int copy_row(const color_t color, const size_t src, const size_t dst)
{
    size_t* score;
    size_t i;

    switch (color) {
        case 'B': 
            score = &g_black_score;
            break;
        case 'W':
            score = &g_white_score;
            break;
        default:
            return FALSE;
    }
    
    if (src >= g_board_row || dst >= g_board_row || *score < 4) {
        return FALSE;
    }

    for (i = 0; i < g_board_col; ++i) {
        g_board[dst][i] = g_board[src][i];
    }

    *score -= 4;
    return TRUE;
}

int copy_column(const color_t color, const size_t src, const size_t dst)
{
    size_t* score;
    size_t i;

    switch (color) {
        case 'B': 
            score = &g_black_score;
            break;
        case 'W':
            score = &g_white_score;
            break;
        default:
            return FALSE;
    }
    
    if (src >= g_board_col || dst >= g_board_col || *score < 4) {
        return FALSE;
    }

    for (i = 0; i < g_board_col; ++i) {
        g_board[i][dst] = g_board[i][src];
    }

    *score -= 4;
    return TRUE;
}

/* update score */

void update_score(const color_t color, const size_t row, const size_t col) {
    size_t* score;
    size_t i;
    int count = 1;  /* declare possible? */

    score = (color == 'B') ? &g_black_score : &g_white_score;

    /* check horizontal */
    for (i = col + 1; i < g_board_col; ++i) {
        if (g_board[row][i] == color) {
            count++;
        } else {
            break;
        }
    }
   
    for (i = col - 1; (int)i >= 0; --i) {
        if (g_board[row][i] == color) {
            count++;
        } else {
            break;
        }
    }
    
    if (count >= 5) {
        *score += count - 4;
    }
 
    count = 1;
 
    /* check vertical */
    for (i = row + 1; i < g_board_row; ++i) {
        if (g_board[i][col] == color) {
            count++;
        } else {
            break;
        }
    }
   
    for (i = row - 1; (int)i >= 0; --i) {
        if (g_board[i][col] == color) {
            count++;
        } else {
            break;
        }
    }
    
    if (count >= 5) {
        *score += count - 4;
    }
 
    count = 1;    
    
    /* check diagonal */
    for (i = 1; row + i < g_board_row && (col + i < g_board_col); ++i) {     /* if no (), still work? */
        if (g_board[row + i][col + i] == color) {
            count++;
        } else {
            break;
        }    
    }
   
    for (i = 1; (int)row - i >= 0 && ((int)col - i >= 0); ++i) {     
        if (g_board[row - i][col - i] == color) {
            count++;
        } else {
            break;
        }    
    }
    
    if (count >= 5) {
        *score += count - 4;
    }
 
    count = 1;

    for (i = 1; (int)row - i >= 0 && (col + i < g_board_col); ++i) {     
        if (g_board[row - i][col + i] == color) {
            count++;
        } else {
            break;
        }    
    }
   
    for (i = 1; (row + i < g_board_row) && ((int)col - i >= 0); ++i) {     
        if (g_board[row + i][col - i] == color) {
            count++;
        } else {
            break;
        }    
    }
    
    if (count >= 5) {
        *score += count - 4;
    } 
}
