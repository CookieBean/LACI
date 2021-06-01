//
//  main.cpp
//  LACI_Compiler
//
//  Created by 박상권 on 2021/05/27.
//

#include <iostream>
#include <vector>
#include <unistd.h>
using namespace std;
void decision();
void evenodd();
void operation();
void prevset();
FILE* input_code = fopen("input.txt", "r");
int EAX, EBX, EIP, COPY, next_EIP = -1;
char prev_F = '.', prev_E = '.';
struct CODE {
    char c[3];
};
vector<CODE> code;
vector<int> S;
void decision() {
    CODE tmp = code[EIP];
    if(tmp.c[0] == 'L') {
        if(prev_F == 'A') {
            if(!S.empty()) {
                if(S.back() == 0) next_EIP = EBX;
            }
        }
        if(prev_F == 'C') {
            if(!S.empty()) {
                COPY = S.back();
            }
        }
    } else if(tmp.c[0] == 'A') {
        if(prev_F == 'I') {
            while(!S.empty()) {
                S.pop_back();
            }
        }
    } else if(tmp.c[0] == 'C') {
        if(prev_F == 'L') {
            if(!S.empty()) {
                if(EAX > 0) {
                    EAX--;
                    next_EIP = EBX;
                }
            }
        } else if(prev_F == 'I') {
            if(!S.empty()) {
                int tmp = S.back() + (1<<7);
                S.pop_back();
                S.push_back(tmp);
            }
        }
    } else if(tmp.c[0] == 'I') {
        if(prev_F == 'L') {
            S.push_back(COPY);
        } else if(prev_F == 'A') {
            if(S.size() >= 2) {
                int tmp1 = S[S.size()-1];
                int tmp2 = S[S.size()-2];
                if(tmp1 == tmp2) S.push_back(1);
                else S.push_back(0);
            }
        }
    }
    if(next_EIP != -1) {
        EIP = next_EIP;
        next_EIP = -1;
    } else {
        evenodd();
    }
}
void evenodd() {
    CODE tmp = code[EIP];
    int cur_E = tmp.c[1] == 'L' || tmp.c[1] == 'A' ? 'E' : 'O';
    if(prev_E == cur_E) {
        if(prev_E == 'E') {
            if(!S.empty()) S[S.size()-1] = S.back() << 1;
        }
        if(prev_E == 'O') {
            if(!S.empty()) S[S.size()-1] = S.back() >> 1;
        }
    }
    operation();
}
void operation() {
    CODE tmp = code[EIP];
    if(tmp.c[0] == 'L') {
        if(tmp.c[1] == 'L' || tmp.c[1] == 'C') {
            if(tmp.c[2] == 'L') {
                if(!S.empty()) {
                    EAX = S.back();
                }
            } else if(tmp.c[2] == 'A') {
                S.push_back(EAX);
            } else if(tmp.c[2] == 'C') {
                if(!S.empty()) {
                    EBX = S.back();
                }
            } else if(tmp.c[2] == 'I') {
                S.push_back(EBX);
            }
        }
        if(tmp.c[1] == 'A' || tmp.c[1] == 'I') {
            if(tmp.c[2] == 'L') {
                EBX = EIP+1;
            } else if(tmp.c[2] == 'A') {
                exit(0);
            }
        }
    } else if(tmp.c[0] == 'A') {
        if(tmp.c[1] == 'L' || tmp.c[1] == 'C') {
            if(tmp.c[2] == 'L') {
                if(S.size() >= 2) {
                    int tmp2 = S.back();
                    S.pop_back();
                    int tmp1 = S.back();
                    S.pop_back();
                    S.push_back(tmp1+tmp2);
                }
            } else if(tmp.c[2] == 'A') {
                if(S.size() >= 2) {
                    int tmp2 = S.back();
                    S.pop_back();
                    int tmp1 = S.back();
                    S.pop_back();
                    S.push_back(tmp1-tmp2);
                }
            } else if(tmp.c[2] == 'C') {
                if(S.size() >= 2) {
                    int tmp2 = S.back();
                    S.pop_back();
                    int tmp1 = S.back();
                    S.pop_back();
                    S.push_back(tmp1*tmp2);
                }
            } else if(tmp.c[2] == 'I') {
                if(S.size() >= 2) {
                    int tmp2 = S.back();
                    S.pop_back();
                    int tmp1 = S.back();
                    S.pop_back();
                    S.push_back(tmp1/tmp2);
                }
            }
        }
        if(tmp.c[1] == 'A' || tmp.c[1] == 'I') {
            if(tmp.c[2] == 'L') {
                if(!S.empty()) {
                    S[S.size()-1] = S.back()+1;
                }
            } else if(tmp.c[2] == 'A') {
                if(!S.empty()) {
                    S[S.size()-1] = S.back()-1;
                }
            } else if(tmp.c[2] == 'I') {
                if(!S.empty()) S.pop_back();
            }
        }
    } else if(tmp.c[0] == 'C') {
        if(tmp.c[1] == 'L' || tmp.c[1] == 'C') {
            if(tmp.c[2] == 'L') {
                if(S.size() >= 2) {
                    int tmp2 = S.back();
                    S.pop_back();
                    int tmp1 = S.back();
                    S.pop_back();
                    S.push_back(tmp1 & tmp2);
                }
            } else if(tmp.c[2] == 'A') {
                if(S.size() >= 2) {
                    int tmp2 = S.back();
                    S.pop_back();
                    int tmp1 = S.back();
                    S.pop_back();
                    S.push_back(tmp1 | tmp2);
                }
            } else if(tmp.c[2] == 'C') {
                if(S.size() >= 1) {
                    int tmp1 = S.back();
                    S.pop_back();
                    S.push_back(!tmp1);
                }
            } else if(tmp.c[2] == 'I') {
                if(S.size() >= 2) {
                    int tmp2 = S.back();
                    S.pop_back();
                    int tmp1 = S.back();
                    S.pop_back();
                    S.push_back(tmp1 ^ tmp2);
                }
            }
        }
        if(tmp.c[1] == 'A' || tmp.c[1] == 'I') {
            if(tmp.c[2] == 'L') {
                S.push_back(0);
            } else if(tmp.c[2] == 'A') {
                S.push_back(S.empty());
            } else if(tmp.c[2] == 'I') {
                if(S.size() >= 2) {
                    int tmp2 = S.back();
                    S.pop_back();
                    int tmp1 = S.back();
                    S.pop_back();
                    S.push_back(tmp1 < tmp2);
                }
            }
        }
    } else if(tmp.c[0] == 'I') {
        if(tmp.c[1] == 'L' || tmp.c[1] == 'C') {
            if(tmp.c[2] == 'L') {
                int tmp1;
                scanf("%d", &tmp1);
                S.push_back(tmp1);
            } else if(tmp.c[2] == 'A') {
                char tmp1;
                scanf("%c", &tmp1);
                S.push_back((int)tmp1);
            } else if(tmp.c[2] == 'C') {
                printf("%d", S.back());
            } else if(tmp.c[2] == 'I') {
                printf("%c", (char)S.back());
            }
        }
        if(tmp.c[1] == 'A' || tmp.c[1] == 'I') {
            if(tmp.c[2] == 'L') {
                printf("%d", S.back());
                S.pop_back();
            } else if(tmp.c[2] == 'A') {
                printf("%c", (char)S.back());
                S.pop_back();
            }
        }
    }
    prevset();
}
void prevset() {
    prev_F = code[EIP].c[0];
    prev_E = code[EIP].c[1] == 'L' || code[EIP].c[1] == 'A' ? 'E' : 'O';
    if(next_EIP == -1) EIP++;
    else EIP = next_EIP;
    next_EIP = -1;
}
int main(int argc, const char * argv[]) {
    CODE tmp;
    while(fscanf(input_code, "%3s", tmp.c) != EOF) {
        code.push_back(tmp);
    }
    while(1) {
        decision();
//        S;
//        next_EIP = next_EIP;
//        sleep(1);
    }
}
