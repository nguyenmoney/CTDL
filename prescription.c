#include "prescription.h"
#include "medicine.h" // De su dung timThuocTheoMa va inDanhSachThuoc
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "examination.h" // De su dung mucDoUuTienToString
// Ham tien ich noi bo
static void removeNewlinePresc(char* str) {
    if (str == NULL) return;
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

static void readStringSafePresc(char* buffer, int size) {
    if (fgets(buffer, size, stdin) != NULL) {
        removeNewlinePresc(buffer);
    } else {
        buffer[0] = '\0';
    }
}

DonThuocChiTiet* docDanhSachDonThuocChiTiet(const char* tenFile, int* soLuongDongConTro) {
    FILE* f = fopen(tenFile, "r");
    if (f == NULL) {
        *soLuongDongConTro = 0;
        return NULL;
    }

    DonThuocChiTiet* ds = NULL;
    int capacity = 0;
    *soLuongDongConTro = 0;
    char line[500];

    // Bo qua dong tieu de (neu co)
    // fgets(line, sizeof(line), f);

    while (fgets(line, sizeof(line), f) != NULL) {
        removeNewlinePresc(line);
        if (strlen(line) < 5) continue;

        if (*soLuongDongConTro >= capacity) {
            capacity = (capacity == 0) ? 10 : capacity * 2;
            DonThuocChiTiet* temp = realloc(ds, capacity * sizeof(DonThuocChiTiet));
            if (temp == NULL) {
                perror("Loi realloc (docDanhSachDonThuocChiTiet)");
                free(ds); fclose(f); *soLuongDongConTro = 0; return NULL;
            }
            ds = temp;
        }

        DonThuocChiTiet dtct;
        // Dinh dang CSV: maDonThuoc,maThuoc,soLuong,lieuDung,cachDung
        char* token;

        token = strtok(line, ","); if (token) dtct.maDonThuocChiTiet = atoi(token); else continue;
        
        token = strtok(NULL, ","); if (token) strncpy(dtct.maThuoc, token, sizeof(dtct.maThuoc)-1); else strcpy(dtct.maThuoc, "");
        dtct.maThuoc[sizeof(dtct.maThuoc)-1] = '\0';
        
        token = strtok(NULL, ","); if (token) dtct.soLuong = atoi(token); else dtct.soLuong = 0;
        
        token = strtok(NULL, ","); if (token) strncpy(dtct.lieuDung, token, sizeof(dtct.lieuDung)-1); else strcpy(dtct.lieuDung, "");
        dtct.lieuDung[sizeof(dtct.lieuDung)-1] = '\0';

        token = strtok(NULL, "\n"); if (token) strncpy(dtct.cachDung, token, sizeof(dtct.cachDung)-1); else strcpy(dtct.cachDung, "");
        dtct.cachDung[sizeof(dtct.cachDung)-1] = '\0';
        
        ds[*soLuongDongConTro] = dtct;
        (*soLuongDongConTro)++;
    }
    fclose(f);
    return ds;
}

void ghiDanhSachDonThuocChiTietRaFile(const char* tenFile, const DonThuocChiTiet dsDonThuocChiTiet[], int soLuongDong) {
    FILE* f = fopen(tenFile, "w");
    if (f == NULL) {
        perror("Loi mo file (ghiDanhSachDonThuocChiTietRaFile)");
        return;
    }
    // fprintf(f, "MaDonThuoc,MaThuoc,SoLuong,LieuDung,CachDung\n");

    for (int i = 0; i < soLuongDong; ++i) {
        fprintf(f, "%d,%s,%d,%s,%s\n",
                dsDonThuocChiTiet[i].maDonThuocChiTiet,
                dsDonThuocChiTiet[i].maThuoc,
                dsDonThuocChiTiet[i].soLuong,
                dsDonThuocChiTiet[i].lieuDung,
                dsDonThuocChiTiet[i].cachDung);
    }
    fclose(f);
}

int themDongDonThuocChiTiet(DonThuocChiTiet** dsDTCTConTro, int* soLuongDongConTro, DonThuocChiTiet dtctMoi, const char* tenFile) {
    // Kiem tra xem thuoc da co trong don thuoc nay chua
    for (int i = 0; i < *soLuongDongConTro; ++i) {
        if ((*dsDTCTConTro)[i].maDonThuocChiTiet == dtctMoi.maDonThuocChiTiet &&
            strcmp((*dsDTCTConTro)[i].maThuoc, dtctMoi.maThuoc) == 0) {
            printf("Loi: Thuoc '%s' da ton tai trong don thuoc %d. Hay sua so luong neu can.\n", dtctMoi.maThuoc, dtctMoi.maDonThuocChiTiet);
            return 0; // Thuoc da ton tai
        }
    }

    DonThuocChiTiet* temp = realloc(*dsDTCTConTro, (*soLuongDongConTro + 1) * sizeof(DonThuocChiTiet));
    if (temp == NULL) {
        perror("Loi realloc (themDongDonThuocChiTiet)");
        return 0;
    }
    *dsDTCTConTro = temp;
    (*dsDTCTConTro)[*soLuongDongConTro] = dtctMoi;
    (*soLuongDongConTro)++;
    ghiDanhSachDonThuocChiTietRaFile(tenFile, *dsDTCTConTro, *soLuongDongConTro);
    return 1;
}

int xoaDongThuocTrongDon(DonThuocChiTiet** dsDTCTConTro, int* soLuongDongConTro, int maDonThuocCanTim, const char* maThuocCanXoa, const char* tenFile) {
    int viTriCanXoa = -1;
    for (int i = 0; i < *soLuongDongConTro; ++i) {
        if ((*dsDTCTConTro)[i].maDonThuocChiTiet == maDonThuocCanTim &&
            strcmp((*dsDTCTConTro)[i].maThuoc, maThuocCanXoa) == 0) {
            viTriCanXoa = i;
            break;
        }
    }

    if (viTriCanXoa != -1) {
        for (int i = viTriCanXoa; i < (*soLuongDongConTro - 1); ++i) {
            (*dsDTCTConTro)[i] = (*dsDTCTConTro)[i + 1];
        }
        (*soLuongDongConTro)--;
        if (*soLuongDongConTro == 0) {
            free(*dsDTCTConTro);
            *dsDTCTConTro = NULL;
        } else {
            DonThuocChiTiet* temp = realloc(*dsDTCTConTro, (*soLuongDongConTro) * sizeof(DonThuocChiTiet));
            if (temp == NULL && *soLuongDongConTro > 0) { /* Xu ly loi realloc neu can */ }
            else if(*soLuongDongConTro > 0) *dsDTCTConTro = temp;
        }
        ghiDanhSachDonThuocChiTietRaFile(tenFile, *dsDTCTConTro, *soLuongDongConTro);
        return 1;
    }
    printf("Khong tim thay thuoc '%s' trong don thuoc %d de xoa.\n", maThuocCanXoa, maDonThuocCanTim);
    return 0;
}

int suaDongThuocTrongDon(DonThuocChiTiet dsDonThuocChiTiet[], int soLuongDong, int maDonThuocCanTim, const char* maThuocCanSua, DonThuocChiTiet thongTinMoi, const char* tenFile) {
    for (int i = 0; i < soLuongDong; ++i) {
        if (dsDonThuocChiTiet[i].maDonThuocChiTiet == maDonThuocCanTim &&
            strcmp(dsDonThuocChiTiet[i].maThuoc, maThuocCanSua) == 0) {
            // Ma don thuoc va ma thuoc khong doi, chi doi so luong, lieu dung, cach dung
            dsDonThuocChiTiet[i].soLuong = thongTinMoi.soLuong;
            strncpy(dsDonThuocChiTiet[i].lieuDung, thongTinMoi.lieuDung, sizeof(dsDonThuocChiTiet[i].lieuDung)-1);
            dsDonThuocChiTiet[i].lieuDung[sizeof(dsDonThuocChiTiet[i].lieuDung)-1] = '\0';
            strncpy(dsDonThuocChiTiet[i].cachDung, thongTinMoi.cachDung, sizeof(dsDonThuocChiTiet[i].cachDung)-1);
            dsDonThuocChiTiet[i].cachDung[sizeof(dsDonThuocChiTiet[i].cachDung)-1] = '\0';
            
            ghiDanhSachDonThuocChiTietRaFile(tenFile, dsDonThuocChiTiet, soLuongDong);
            return 1;
        }
    }
    printf("Khong tim thay thuoc '%s' trong don thuoc %d de sua.\n", maThuocCanSua, maDonThuocCanTim);
    return 0;
}


void inChiTietMotDonThuocConsole(int maDonThuoc,
                                 const DonThuocChiTiet dsDonThuocChiTiet[], int soLuongDongTongCong,
                                 const Thuoc dsThuocToanBo[], int soLuongThuocTB) {
    printf("\n--- CHI TIET DON THUOC MA: %d ---\n", maDonThuoc);
    int stt = 0;
    int timThayDon = 0;

    for (int i = 0; i < soLuongDongTongCong; ++i) {
        if (dsDonThuocChiTiet[i].maDonThuocChiTiet == maDonThuoc) {
            if (!timThayDon) { // In header chi mot lan cho don thuoc
                printf("----------------------------------------------------------------------------------------------------\n");
                printf("| %-3s | %-15s | %-30s | %-7s | %-20s | %-20s |\n", "STT", "Ma Thuoc", "Ten Thuoc", "So Luong", "Lieu Dung", "Cach Dung");
                printf("----------------------------------------------------------------------------------------------------\n");
                timThayDon = 1;
            }
            stt++;
            const Thuoc* thuocInfo = timThuocTheoMa(dsThuocToanBo, soLuongThuocTB, dsDonThuocChiTiet[i].maThuoc);
            char tenThuocHienThi[151] = "Khong tim thay";
            if (thuocInfo != NULL) {
                strncpy(tenThuocHienThi, thuocInfo->tenThuoc, 150);
                tenThuocHienThi[150] = '\0';
            }

            printf("| %-3d | %-15s | %-30.30s | %-7d | %-20.20s | %-20.20s |\n",
                   stt,
                   dsDonThuocChiTiet[i].maThuoc,
                   tenThuocHienThi,
                   dsDonThuocChiTiet[i].soLuong,
                   dsDonThuocChiTiet[i].lieuDung,
                   dsDonThuocChiTiet[i].cachDung);
        }
    }

    if (!timThayDon) {
        printf("Khong co chi tiet nao cho don thuoc ma %d hoac don thuoc khong ton tai.\n", maDonThuoc);
    } else {
        printf("----------------------------------------------------------------------------------------------------\n");
    }
}

void inTatCaDonThuocChiTietConsole(const DonThuocChiTiet dsDonThuocChiTiet[], int soLuongDong,
                                   const Thuoc dsThuocToanBo[], int soLuongThuocTB) {
    printf("\n--- TAT CA CHI TIET CAC DON THUOC ---\n");
    if (soLuongDong == 0) {
        printf("Khong co chi tiet don thuoc nao.\n");
        return;
    }
    printf("----------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-7s | %-15s | %-30s | %-7s | %-20s | %-20s |\n", "Ma DT", "Ma Thuoc", "Ten Thuoc", "So Luong", "Lieu Dung", "Cach Dung");
    printf("----------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < soLuongDong; ++i) {
        const Thuoc* thuocInfo = timThuocTheoMa(dsThuocToanBo, soLuongThuocTB, dsDonThuocChiTiet[i].maThuoc);
        char tenThuocHienThi[151] = "N/A";
        if (thuocInfo != NULL) {
            strncpy(tenThuocHienThi, thuocInfo->tenThuoc, 150);
             tenThuocHienThi[150] = '\0';
        }
        printf("| %-7d | %-15s | %-30.30s | %-7d | %-20.20s | %-20.20s |\n",
               dsDonThuocChiTiet[i].maDonThuocChiTiet,
               dsDonThuocChiTiet[i].maThuoc,
               tenThuocHienThi,
               dsDonThuocChiTiet[i].soLuong,
               dsDonThuocChiTiet[i].lieuDung,
               dsDonThuocChiTiet[i].cachDung);
    }
    printf("----------------------------------------------------------------------------------------------------------------------\n");
}


void quanLyCapNhatChiTietDonThuoc(int maDonThuocDeCapNhat,
                                  DonThuocChiTiet** dsDTCTConTro, int* soLuongDongConTro,
                                  const Thuoc dsThuocToanBo[], int soLuongThuocTB,
                                  const char* tenFileCSV) {
    int choice;
    char buffer[100];

    if (dsThuocToanBo == NULL || soLuongThuocTB == 0) {
        printf("Khong co du lieu thuoc de ke don. Vui long cap nhat danh sach thuoc truoc.\n");
        return;
    }

    do {
        inChiTietMotDonThuocConsole(maDonThuocDeCapNhat, *dsDTCTConTro, *soLuongDongConTro, dsThuocToanBo, soLuongThuocTB);
        printf("\n--- QUAN LY DON THUOC MA: %d ---\n", maDonThuocDeCapNhat);
        printf("1. Them thuoc vao don\n");
        printf("2. Sua thong tin thuoc trong don\n");
        printf("3. Xoa thuoc khoi don\n");
        printf("0. Quay lai\n");
        printf("Lua chon cua ban: ");
        
        readStringSafePresc(buffer, sizeof(buffer));
        if (sscanf(buffer, "%d", &choice) != 1) {
            choice = -1; // Lua chon khong hop le
        }

        switch (choice) {
            case 1: { // Them thuoc
                DonThuocChiTiet dtctMoi;
                dtctMoi.maDonThuocChiTiet = maDonThuocDeCapNhat;

                printf("\n--- Danh Sach Thuoc Hien Co ---\n");
                // Gia su co ham inDanhSachThuocConsole tu medicine.c
                // void inDanhSachThuocConsole(const Thuoc ds[], int soLuong); // Khai bao neu chua co
                inDanhSachThuocConsole(dsThuocToanBo, soLuongThuocTB); // Goi ham tu medicine.c

                printf("Nhap Ma Thuoc can them: ");
                readStringSafePresc(dtctMoi.maThuoc, sizeof(dtctMoi.maThuoc));
                for(int i=0; dtctMoi.maThuoc[i]; i++) dtctMoi.maThuoc[i] = toupper(dtctMoi.maThuoc[i]);


                if (timThuocTheoMa(dsThuocToanBo, soLuongThuocTB, dtctMoi.maThuoc) == NULL) {
                    printf("Loi: Ma Thuoc '%s' khong ton tai trong danh muc thuoc!\n", dtctMoi.maThuoc);
                    break;
                }

                printf("Nhap So Luong: ");
                if (scanf("%d", &dtctMoi.soLuong) != 1 || dtctMoi.soLuong <= 0) {
                    printf("So luong khong hop le.\n");
                    int c_temp_buffer; 
while ((c_temp_buffer = getchar()) != '\n' && c_temp_buffer != EOF);
break;
                }
                while(getchar()!='\n'); 

                printf("Nhap Lieu Dung: "); readStringSafePresc(dtctMoi.lieuDung, sizeof(dtctMoi.lieuDung));
                printf("Nhap Cach Dung: "); readStringSafePresc(dtctMoi.cachDung, sizeof(dtctMoi.cachDung));

                if (themDongDonThuocChiTiet(dsDTCTConTro, soLuongDongConTro, dtctMoi, tenFileCSV)) {
                    printf("Them thuoc vao don thanh cong!\n");
                }
                break;
            }
            case 2: { // Sua thuoc
                char maThuocSua[30];
                DonThuocChiTiet thongTinMoi;
                printf("Nhap Ma Thuoc trong don can sua: ");
                readStringSafePresc(maThuocSua, sizeof(maThuocSua));
                for(int i=0; maThuocSua[i]; i++) maThuocSua[i] = toupper(maThuocSua[i]);


                // Kiem tra thuoc co trong don khong
                int timThayThuocTrongDon = 0;
                for(int i=0; i< *soLuongDongConTro; ++i) {
                    if ((*dsDTCTConTro)[i].maDonThuocChiTiet == maDonThuocDeCapNhat && strcmp((*dsDTCTConTro)[i].maThuoc, maThuocSua) == 0) {
                        timThayThuocTrongDon = 1;
                        break;
                    }
                }
                if(!timThayThuocTrongDon) {
                    printf("Thuoc '%s' khong co trong don thuoc %d.\n", maThuocSua, maDonThuocDeCapNhat);
                    break;
                }

                printf("Nhap So Luong moi: ");
                if (scanf("%d", &thongTinMoi.soLuong) != 1 || thongTinMoi.soLuong <=0) {
                     printf("So luong khong hop le.\n");
                     while(getchar()!='\n'); 
                     break;
                }
                while(getchar()!='\n');

                printf("Nhap Lieu Dung moi: "); readStringSafePresc(thongTinMoi.lieuDung, sizeof(thongTinMoi.lieuDung));
                printf("Nhap Cach Dung moi: "); readStringSafePresc(thongTinMoi.cachDung, sizeof(thongTinMoi.cachDung));
                
                if (suaDongThuocTrongDon(*dsDTCTConTro, *soLuongDongConTro, maDonThuocDeCapNhat, maThuocSua, thongTinMoi, tenFileCSV)) {
                    printf("Sua thong tin thuoc thanh cong!\n");
                }
                break;
            }
            case 3: { // Xoa thuoc
                char maThuocXoa[30];
                printf("Nhap Ma Thuoc trong don can xoa: ");
                readStringSafePresc(maThuocXoa, sizeof(maThuocXoa));
                 for(int i=0; maThuocXoa[i]; i++) maThuocXoa[i] = toupper(maThuocXoa[i]);

                if (xoaDongThuocTrongDon(dsDTCTConTro, soLuongDongConTro, maDonThuocDeCapNhat, maThuocXoa, tenFileCSV)) {
                    printf("Xoa thuoc khoi don thanh cong!\n");
                }
                break;
            }
            case 0:
                printf("Quay lai menu truoc.\n");
                break;
            default:
                printf("Lua chon khong hop le. Vui long chon lai.\n");
                break;
        }
        if (choice != 0) {
             printf("Nhan Enter de tiep tuc...");
             getchar(); // Cho nguoi dung doc thong bao
        }
    } while (choice != 0);
}