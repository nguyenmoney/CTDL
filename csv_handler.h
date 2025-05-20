#ifndef CSV_HANDLER_H
#define CSV_HANDLER_H

#include <stdio.h> // Cho FILE* va cac dinh nghia I/O

// --- Khai bao cac ham tien ich chung ---

/**
 * @brief Mo mot file voi che do chi dinh, kem theo kiem tra loi co ban.
 *
 * @param tenFile Duong dan toi file can mo.
 * @param cheDo Chuoi che do mo file (vi du: "r", "w", "a", "rb", "wb", "ab",...).
 * @return Con tro FILE neu mo thanh cong, NULL neu that bai (da in thong bao loi).
 */
FILE* moFileAnToan(const char* tenFile, const char* cheDo);

/**
 * @brief Dong mot file da mo, kem theo kiem tra con tro FILE.
 *
 * @param f Con tro FILE can dong.
 */
void dongFileAnToan(FILE* f);

/**
 * @brief Loai bo ky tu xuong dong ('\n') o cuoi chuoi (neu co).
 * Ham nay thay doi truc tiep chuoi dau vao.
 *
 * @param str Chuoi can xu ly.
 */
void xoaKyTuXuongDong(char* str);

/**
 * @brief Doc mot dong tu stream dau vao (vi du: stdin hoac file) mot cach an toan.
 * Su dung fgets va sau do goi xoaKyTuXuongDong.
 * Dam bao chuoi ket thuc bang null.
 *
 * @param buffer Bo dem de luu tru chuoi doc duoc.
 * @param kichThuocBuffer Kich thuoc cua bo dem.
 * @param stream Stream dau vao (vi du: stdin, con tro FILE).
 * @return Con tro toi buffer neu doc thanh cong, NULL neu co loi hoac EOF ma khong doc duoc gi.
 * Neu loi hoac EOF, buffer[0] se la '\0'.
 */
char* docDongAnToan(char* buffer, int kichThuocBuffer, FILE* stream);

/**
 * @brief Dem so luong dong trong mot file CSV.
 * Co the tuy chon bo qua dong tieu de.
 *
 * @param tenFile Ten file CSV.
 * @param boQuaDongTieuDe 1 neu bo qua dong dau tien (tieu de), 0 neu khong.
 * @return So luong dong dem duoc, hoac -1 neu co loi khi mo file.
 */
int demSoDongTrongFile(const char* tenFile, int boQuaDongTieuDe);

// Cac ham xu ly CSV cu the cho tung struct (BenhNhan, LanKham,...) van nam trong
// cac module tuong ung (patient.c, examination.c,...) vi chung phu thuoc
// vao cau truc du lieu cu the do. Module nay cung cap cac tien ich co ban hon.
void rutGonChuoiHienThi(char* outputBuffer, int bufferSize, const char* originalString, int displayWidth);
#endif // CSV_HANDLER_H