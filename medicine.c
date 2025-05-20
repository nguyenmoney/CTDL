#include "medicine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Cho toupper

// Ham tien ich noi bo (static)
static void removeNewlineMed(char* str) {
    if (str == NULL) return;
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

static void readStringSafeMed(char* buffer, int size) {
    if (fgets(buffer, size, stdin) != NULL) {
        removeNewlineMed(buffer);
    } else {
        buffer[0] = '\0';
    }
}

Thuoc* docDanhSachThuoc(const char* tenFile, int* soLuong) {
    FILE* f = fopen(tenFile, "r");
    if (f == NULL) {
        *soLuong = 0;
        return NULL; // Tra ve NULL neu file khong ton tai
    }

    Thuoc* ds = NULL;
    int capacity = 0;
    *soLuong = 0;
    char line[200]; // Buffer cho mot dong CSV thuoc (maThuoc[30] + tenThuoc[150] + separators)

    // Bo qua dong tieu de (neu co)
    // fgets(line, sizeof(line), f); 

    while (fgets(line, sizeof(line), f) != NULL) {
        removeNewlineMed(line);
        if (strlen(line) < 2) continue; // Dong qua ngan

        if (*soLuong >= capacity) {
            capacity = (capacity == 0) ? 10 : capacity * 2; // Khoi tao voi 10 loai thuoc
            Thuoc* temp = realloc(ds, capacity * sizeof(Thuoc));
            if (temp == NULL) {
                perror("Loi cap phat bo nho (realloc docDanhSachThuoc)");
                free(ds);
                fclose(f);
                *soLuong = 0;
                return NULL;
            }
            ds = temp;
        }

        Thuoc t;
        // Dinh dang CSV: maThuoc,tenThuoc
        char* token;
        
        token = strtok(line, ","); 
        if (token) {
            strncpy(t.maThuoc, token, sizeof(t.maThuoc)-1);
            t.maThuoc[sizeof(t.maThuoc)-1] = '\0';
        } else {
            continue; // Bo qua dong khong co ma thuoc
        }

        token = strtok(NULL, "\n"); // Doc den het dong cho tenThuoc
        if (token) {
            strncpy(t.tenThuoc, token, sizeof(t.tenThuoc)-1);
            t.tenThuoc[sizeof(t.tenThuoc)-1] = '\0';
        } else {
            strcpy(t.tenThuoc, "N/A"); // Gia tri mac dinh neu ten thuoc trong
        }
        
        ds[*soLuong] = t;
        (*soLuong)++;
    }

    fclose(f);
    return ds;
}

void ghiDanhSachThuocRaFile(const char* tenFile, const Thuoc dsThuoc[], int soLuong) {
    FILE* f = fopen(tenFile, "w"); 
    if (f == NULL) {
        perror("Loi mo file de ghi (ghiDanhSachThuocRaFile)");
        return;
    }
    // fprintf(f, "MaThuoc,TenThuoc\n");

    for (int i = 0; i < soLuong; ++i) {
        fprintf(f, "%s,%s\n",
                dsThuoc[i].maThuoc,
                dsThuoc[i].tenThuoc);
    }
    fclose(f);
}

Thuoc* timThuocTheoMa(const Thuoc dsThuoc[], int soLuong, const char* maThuoc) {
    if (maThuoc == NULL) return NULL;
    char maThuocUpper[30];
    strncpy(maThuocUpper, maThuoc, sizeof(maThuocUpper)-1);
    maThuocUpper[sizeof(maThuocUpper)-1] = '\0';
    for(int i=0; maThuocUpper[i]; i++) maThuocUpper[i] = toupper(maThuocUpper[i]);


    for (int i = 0; i < soLuong; ++i) {
        char currentMaThuocUpper[30];
        strncpy(currentMaThuocUpper, dsThuoc[i].maThuoc, sizeof(currentMaThuocUpper)-1);
        currentMaThuocUpper[sizeof(currentMaThuocUpper)-1] = '\0';
        for(int k=0; currentMaThuocUpper[k]; k++) currentMaThuocUpper[k] = toupper(currentMaThuocUpper[k]);

        if (strcmp(currentMaThuocUpper, maThuocUpper) == 0) {
            return (Thuoc*)&dsThuoc[i]; 
        }
    }
    return NULL; // Khong tim thay
}

int nhapThongTinThuocTuBanPhim(Thuoc* tConTro, const Thuoc dsThuocHienCo[], int soLuongThuocHienCo) {
    char bufferMa[sizeof(tConTro->maThuoc)];
    printf("Nhap Ma Thuoc (VD: PARA500, toi da %zu ky tu, se duoc chuyen thanh IN HOA): ", sizeof(tConTro->maThuoc)-1);
    readStringSafeMed(bufferMa, sizeof(bufferMa));
    
    for(int i = 0; bufferMa[i]; i++){ // Chuyen ma thuoc sang chu hoa
        bufferMa[i] = toupper(bufferMa[i]);
    }

    if (strlen(bufferMa) == 0) {
        printf("Ma Thuoc khong duoc de trong.\n");
        return 0;
    }
    // Kiem tra ma thuoc da ton tai chua (khong phan biet hoa/thuong)
    if (timThuocTheoMa(dsThuocHienCo, soLuongThuocHienCo, bufferMa) != NULL) {
        printf("Loi: Ma Thuoc '%s' da ton tai!\n", bufferMa);
        return 0; // Ma da ton tai
    }
    strncpy(tConTro->maThuoc, bufferMa, sizeof(tConTro->maThuoc)-1);
    tConTro->maThuoc[sizeof(tConTro->maThuoc)-1] = '\0';

    printf("Nhap Ten Thuoc (toi da %zu ky tu): ", sizeof(tConTro->tenThuoc)-1); 
    readStringSafeMed(tConTro->tenThuoc, sizeof(tConTro->tenThuoc));
    if (strlen(tConTro->tenThuoc) == 0) {
        printf("Ten Thuoc khong duoc de trong.\n");
        return 0;
    }
    return 1; // Nhap thanh cong
}

int themThuoc(Thuoc** dsThuocConTro, int* soLuongConTro, Thuoc thuocMoi, const char* tenFile) {
    // Kiem tra trung ma da duoc thuc hien trong nhapThongTinThuocTuBanPhim
    // Neu can kiem tra lai o day:
    // if (timThuocTheoMa(*dsThuocConTro, *soLuongConTro, thuocMoi.maThuoc) != NULL) {
    //     printf("Loi: Ma Thuoc '%s' da ton tai khi them (kiem tra lai).\n", thuocMoi.maThuoc);
    //     return 0; 
    // }

    Thuoc* temp = realloc(*dsThuocConTro, (*soLuongConTro + 1) * sizeof(Thuoc));
    if (temp == NULL) {
        perror("Loi cap phat bo nho (realloc themThuoc)");
        return 0;
    }
    *dsThuocConTro = temp;

    (*dsThuocConTro)[*soLuongConTro] = thuocMoi;
    (*soLuongConTro)++;

    ghiDanhSachThuocRaFile(tenFile, *dsThuocConTro, *soLuongConTro);
    return 1;
}

int suaThongTinThuoc(Thuoc dsThuoc[], int soLuong, const char* maThuocCanSua, Thuoc thongTinMoi, const char* tenFile) {
    char maThuocCanSuaUpper[30];
    strncpy(maThuocCanSuaUpper, maThuocCanSua, sizeof(maThuocCanSuaUpper)-1);
    maThuocCanSuaUpper[sizeof(maThuocCanSuaUpper)-1] = '\0';
    for(int i=0; maThuocCanSuaUpper[i]; i++) maThuocCanSuaUpper[i] = toupper(maThuocCanSuaUpper[i]);

    for (int i = 0; i < soLuong; ++i) {
        char currentMaThuocUpper[30];
        strncpy(currentMaThuocUpper, dsThuoc[i].maThuoc, sizeof(currentMaThuocUpper)-1);
        currentMaThuocUpper[sizeof(currentMaThuocUpper)-1] = '\0';
        for(int k=0; currentMaThuocUpper[k]; k++) currentMaThuocUpper[k] = toupper(currentMaThuocUpper[k]);

        if (strcmp(currentMaThuocUpper, maThuocCanSuaUpper) == 0) {
            // Ma thuoc khong doi, chi sua ten thuoc
            if (strlen(thongTinMoi.tenThuoc) == 0) {
                printf("Ten thuoc moi khong duoc de trong.\n");
                return 0;
            }
            strncpy(dsThuoc[i].tenThuoc, thongTinMoi.tenThuoc, sizeof(dsThuoc[i].tenThuoc)-1);
            dsThuoc[i].tenThuoc[sizeof(dsThuoc[i].tenThuoc)-1] = '\0';
            
            ghiDanhSachThuocRaFile(tenFile, dsThuoc, soLuong);
            return 1; // Sua thanh cong
        }
    }
    printf("Khong tim thay thuoc voi ma '%s' de sua.\n", maThuocCanSua);
    return 0; // Khong tim thay
}

int xoaThuoc(Thuoc** dsThuocConTro, int* soLuongConTro, const char* maThuocCanXoa, const char* tenFile) {
    int viTriCanXoa = -1;
    char maThuocCanXoaUpper[30];
    strncpy(maThuocCanXoaUpper, maThuocCanXoa, sizeof(maThuocCanXoaUpper)-1);
    maThuocCanXoaUpper[sizeof(maThuocCanXoaUpper)-1] = '\0';
    for(int i=0; maThuocCanXoaUpper[i]; i++) maThuocCanXoaUpper[i] = toupper(maThuocCanXoaUpper[i]);


    for (int i = 0; i < *soLuongConTro; ++i) {
        char currentMaThuocUpper[30];
        strncpy(currentMaThuocUpper, (*dsThuocConTro)[i].maThuoc, sizeof(currentMaThuocUpper)-1);
        currentMaThuocUpper[sizeof(currentMaThuocUpper)-1] = '\0';
        for(int k=0; currentMaThuocUpper[k]; k++) currentMaThuocUpper[k] = toupper(currentMaThuocUpper[k]);

        if (strcmp(currentMaThuocUpper, maThuocCanXoaUpper) == 0) {
            viTriCanXoa = i;
            break;
        }
    }

    if (viTriCanXoa != -1) {
        // TODO: Canh bao neu thuoc nay dang duoc su dung trong bat ky DonThuocChiTiet nao
        // Hien tai, se xoa truc tiep. Day la mot diem can cai thien trong tuong lai
        // de dam bao tinh toan ven du lieu (referential integrity).

        for (int i = viTriCanXoa; i < (*soLuongConTro - 1); ++i) {
            (*dsThuocConTro)[i] = (*dsThuocConTro)[i + 1];
        }
        (*soLuongConTro)--;

        if (*soLuongConTro == 0) {
            free(*dsThuocConTro);
            *dsThuocConTro = NULL;
        } else {
            Thuoc* temp = realloc(*dsThuocConTro, (*soLuongConTro) * sizeof(Thuoc));
            if (temp == NULL && *soLuongConTro > 0) {
                perror("Loi realloc khi xoa thuoc (thu nho mang)");
            } else if (*soLuongConTro > 0) {
                *dsThuocConTro = temp;
            }
        }
        ghiDanhSachThuocRaFile(tenFile, *dsThuocConTro, *soLuongConTro);
        printf("Da xoa thuoc '%s'.\n", maThuocCanXoaUpper);
        return 1; 
    }
    printf("Khong tim thay thuoc voi ma '%s' de xoa.\n", maThuocCanXoa);
    return 0;
}

void inDanhSachThuocConsole(const Thuoc dsThuoc[], int soLuong) {
    printf("\n--- DANH SACH THUOC TRONG HE THONG ---\n");
    if (soLuong == 0) {
        printf("Khong co loai thuoc nao trong danh sach.\n");
        return;
    }
    printf("---------------------------------------------------------------------------\n");
    printf("| %-5s | %-20s | %-40s |\n", "STT", "Ma Thuoc", "Ten Thuoc");
    printf("---------------------------------------------------------------------------\n");

    for (int i = 0; i < soLuong; ++i) {
        printf("| %-5d | %-20s | %-40s |\n",
               i + 1,
               dsThuoc[i].maThuoc,
               dsThuoc[i].tenThuoc);
    }
    printf("---------------------------------------------------------------------------\n");
}