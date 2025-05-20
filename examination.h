#ifndef EXAMINATION_H
#define EXAMINATION_H

#include "structs.h"
#include "patient.h" // Để tương tác với dữ liệu bệnh nhân

#define FILE_LAN_KHAM "data/lankham.csv"

// --- Khai báo các hàm ---

/**
 * @brief Đọc danh sách các lần khám từ file CSV vào một mảng động.
 *
 * @param tenFile Tên file CSV.
 * @param soLuong Con trỏ để lưu trữ số lượng lần khám đã đọc.
 * @return Con trỏ tới mảng LanKham, hoặc NULL nếu lỗi. Người gọi giải phóng bộ nhớ.
 */
LanKham* docDanhSachLanKham(const char* tenFile, int* soLuong);

/**
 * @brief Ghi danh sách lần khám (từ mảng) vào file CSV (ghi đè).
 *
 * @param tenFile Tên file CSV.
 * @param dsLanKham Mảng chứa danh sách lần khám.
 * @param soLuong Số lượng lần khám trong mảng.
 */
void ghiDanhSachLanKhamRaFile(const char* tenFile, const LanKham dsLanKham[], int soLuong);

/**
 * @brief Tạo mã lần khám mới, duy nhất.
 *
 * @param dsLanKham Mảng các lần khám hiện có.
 * @param soLuong Số lượng lần khám.
 * @return Mã lần khám mới.
 */
int taoMaLanKhamMoi(const LanKham dsLanKham[], int soLuong);

/**
 * @brief Đăng ký một lượt khám mới cho bệnh nhân.
 * Nếu bệnh nhân không tồn tại, sẽ yêu cầu tạo mới bệnh nhân.
 * Lần khám mới sẽ có trạng thái LK_DANG_CHO.
 *
 * @param dsLanKhamConTro Con trỏ tới con trỏ mảng LanKham (để realloc).
 * @param soLuongLanKhamConTro Con trỏ tới biến số lượng lần khám.
 * @param dsBenhNhanConTro Con trỏ tới con trỏ mảng BenhNhan (nếu cần tạo bệnh nhân mới).
 * @param soLuongBenhNhanConTro Con trỏ tới biến số lượng bệnh nhân.
 * @param fileLanKhamCSV Tên file CSV của lần khám.
 * @param fileBenhNhanCSV Tên file CSV của bệnh nhân.
 * @return Mã lần khám mới nếu thành công, -1 nếu thất bại.
 */
int dangKyKhamMoi(LanKham** dsLanKhamConTro, int* soLuongLanKhamConTro,
                  BenhNhan** dsBenhNhanConTro, int* soLuongBenhNhanConTro,
                  const char* fileLanKhamCSV, const char* fileBenhNhanCSV);

/**
 * @brief Cập nhật thông tin một lần khám (sau khi bệnh nhân được gọi hoặc khám xong).
 *
 * @param dsLanKham Mảng các lần khám.
 * @param soLuongLanKham Số lượng lần khám.
 * @param maLanKhamCanCapNhat Mã của lần khám cần cập nhật.
 * @param dsBacSi Mảng danh sách bác sĩ (để kiểm tra mã bác sĩ).
 * @param soLuongBacSi Số lượng bác sĩ.
 * @param dsKhoaPhong Mảng danh sách khoa phòng (để kiểm tra mã khoa phòng).
 * @param soLuongKhoaPhong Số lượng khoa phòng.
 * @param fileLanKhamCSV Tên file CSV của lần khám.
 * @return 1 nếu cập nhật thành công, 0 nếu thất bại hoặc không tìm thấy.
 */
int capNhatThongTinLanKham(LanKham dsLanKham[], int soLuongLanKham, int maLanKhamCanCapNhat,
                             const BacSi dsBacSi[], int soLuongBacSi, /* Tạm thời chưa dùng dsBacSi, dsKhoaPhong cho validation */
                             const KhoaPhong dsKhoaPhong[], int soLuongKhoaPhong,
                             const char* fileLanKhamCSV);


/**
 * @brief Thêm một lần khám vào danh sách (ít dùng trực tiếp, chủ yếu qua dangKyKhamMoi).
 *
 * @param dsLanKhamConTro Con trỏ tới con trỏ mảng LanKham.
 * @param soLuongConTro Con trỏ tới biến số lượng lần khám.
 * @param lanKhamMoi Thông tin lần khám mới.
 * @param tenFile Tên file CSV.
 * @return 1 nếu thành công, 0 nếu thất bại.
 */
int themLanKham(LanKham** dsLanKhamConTro, int* soLuongConTro, LanKham lanKhamMoi, const char* tenFile);

/**
 * @brief Xóa một lần khám khỏi danh sách.
 *
 * @param dsLanKhamConTro Con trỏ tới con trỏ mảng LanKham.
 * @param soLuongConTro Con trỏ tới biến số lượng lần khám.
 * @param maLanKhamCanXoa Mã lần khám cần xóa.
 * @param tenFile Tên file CSV.
 * @return 1 nếu thành công, 0 nếu thất bại.
 */
int xoaLanKham(LanKham** dsLanKhamConTro, int* soLuongConTro, int maLanKhamCanXoa, const char* tenFile);

/**
 * @brief In danh sách lần khám ra màn hình console.
 *
 * @param dsLanKham Mảng các lần khám.
 * @param soLuongLanKham Số lượng lần khám.
 * @param dsBenhNhan Mảng các bệnh nhân (để hiển thị tên bệnh nhân).
 * @param soLuongBenhNhan Số lượng bệnh nhân.
 */
void inDanhSachLanKhamConsole(const LanKham dsLanKham[], int soLuongLanKham, const BenhNhan dsBenhNhan[], int soLuongBenhNhan);

/**
 * @brief Tìm kiếm lần khám theo các tiêu chí.
 *
 * @param dsLanKham Mảng lần khám gốc.
 * @param soLuongLanKham Số lượng lần khám.
 * @param tieuChi Chuỗi tiêu chí: "MALANKHAM", "MADINHDANHBN", "TRANGTHAI".
 * @param giaTriTimKiem Giá trị tìm kiếm.
 * @param ketQuaTimKiem Con trỏ lưu mảng kết quả (cấp phát động).
 * @param soLuongKetQua Con trỏ lưu số lượng kết quả.
 * Người gọi giải phóng bộ nhớ ketQuaTimKiem.
 */
void timKiemLanKham(const LanKham dsLanKham[], int soLuongLanKham,
                    const char* tieuChi, const char* giaTriTimKiem,
                    LanKham** ketQuaTimKiem, int* soLuongKetQua);

int suaChiTietLanKham(LanKham dsLanKham[], int soLuongLanKham, int maLanKhamCanSua,
    const BacSi dsBacSi[], int soLuongBacSi,
    const KhoaPhong dsKhoaPhong[], int soLuongKhoaPhong,
    const char* fileLanKhamCSV);
// Hàm tiện ích chuyển đổi enum sang chuỗi và ngược lại
const char* trangThaiLanKhamToString(TrangThaiLanKham tt);
TrangThaiLanKham stringToTrangThaiLanKham(const char* str);
const char* mucDoUuTienToString(MucDoUuTien md);
MucDoUuTien stringToMucDoUuTien(const char* str);

#endif // EXAMINATION_H