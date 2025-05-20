#ifndef MEDICINE_H
#define MEDICINE_H

#include "structs.h" // De su dung Thuoc
#include <stdio.h>

#define FILE_THUOC "data/thuoc.csv"

// --- Khai bao cac ham ---

/**
 * @brief Doc danh sach thuoc tu file CSV vao mot mang dong.
 *
 * @param tenFile Ten file CSV.
 * @param soLuong Con tro de luu tru so luong thuoc da doc.
 * @return Con tro toi mang Thuoc, hoac NULL neu loi. Nguoi goi giai phong bo nho.
 */
Thuoc* docDanhSachThuoc(const char* tenFile, int* soLuong);

/**
 * @brief Ghi danh sach thuoc (tu mang) vao file CSV (ghi de).
 *
 * @param tenFile Ten file CSV.
 * @param dsThuoc Mang chua danh sach thuoc.
 * @param soLuong So luong thuoc trong mang.
 */
void ghiDanhSachThuocRaFile(const char* tenFile, const Thuoc dsThuoc[], int soLuong);

/**
 * @brief Them mot loai thuoc moi vao danh sach (trong bo nho va file CSV).
 *
 * @param dsThuocConTro Con tro toi con tro mang Thuoc (de realloc).
 * @param soLuongConTro Con tro toi bien so luong thuoc.
 * @param thuocMoi Thong tin thuoc moi can them. Ma thuoc phai duy nhat.
 * @param tenFile Ten file CSV de cap nhat.
 * @return 1 neu them thanh cong, 0 neu that bai (vi du: trung ma).
 */
int themThuoc(Thuoc** dsThuocConTro, int* soLuongConTro, Thuoc thuocMoi, const char* tenFile);

/**
 * @brief Sua thong tin mot loai thuoc trong danh sach.
 * Chi sua ten thuoc, ma thuoc khong doi.
 *
 * @param dsThuoc Mang thuoc.
 * @param soLuong So luong thuoc.
 * @param maThuocCanSua Ma thuoc can sua thong tin.
 * @param thongTinMoi Thong tin ten thuoc moi. Ma thuoc trong thongTinMoi se bi bo qua.
 * @param tenFile Ten file CSV.
 * @return 1 neu sua thanh cong, 0 neu khong tim thay hoac loi.
 */
int suaThongTinThuoc(Thuoc dsThuoc[], int soLuong, const char* maThuocCanSua, Thuoc thongTinMoi, const char* tenFile);

/**
 * @brief Xoa mot loai thuoc khoi danh sach.
 * Can than khi xoa thuoc da duoc su dung trong cac don thuoc.
 * (Hien tai, he thong khong kiem tra rang buoc nay, se xoa truc tiep).
 *
 * @param dsThuocConTro Con tro toi con tro mang Thuoc.
 * @param soLuongConTro Con tro toi bien so luong thuoc.
 * @param maThuocCanXoa Ma thuoc can xoa.
 * @param tenFile Ten file CSV.
 * @return 1 neu xoa thanh cong, 0 neu khong tim thay hoac loi.
 */
int xoaThuoc(Thuoc** dsThuocConTro, int* soLuongConTro, const char* maThuocCanXoa, const char* tenFile);

/**
 * @brief In danh sach thuoc ra man hinh console.
 *
 * @param dsThuoc Mang thuoc.
 * @param soLuong So luong thuoc.
 */
void inDanhSachThuocConsole(const Thuoc dsThuoc[], int soLuong);

/**
 * @brief Tim kiem thuoc theo ma.
 *
 * @param dsThuoc Mang thuoc.
 * @param soLuong So luong thuoc.
 * @param maThuoc Ma thuoc can tim.
 * @return Con tro toi Thuoc tim thay, hoac NULL neu khong tim thay.
 */
Thuoc* timThuocTheoMa(const Thuoc dsThuoc[], int soLuong, const char* maThuoc);

/**
 * @brief Nhap thong tin cho mot loai thuoc moi tu ban phim.
 * Ma Thuoc phai la duy nhat.
 * @param tConTro Con tro toi struct Thuoc de dien thong tin.
 * @param dsThuocHienCo Mang cac loai thuoc hien co (de kiem tra trung ma).
 * @param soLuongThuocHienCo So luong thuoc hien co.
 * @return 1 neu nhap thanh cong va ma la duy nhat, 0 neu ma bi trung hoac nhap khong hop le.
 */
int nhapThongTinThuocTuBanPhim(Thuoc* tConTro, const Thuoc dsThuocHienCo[], int soLuongThuocHienCo);

#endif // MEDICINE_H