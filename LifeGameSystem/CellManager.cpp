#include "stdafx.h"
#include "CellManager.h"

namespace LifeGameSystem {

	CellManager::CellManager(int width, int height) :
		BitSize(sizeof(uint_fast64_t) * 8),							// �^��񂩂�v�Z
		_dataSizeWidth(ceil(static_cast<float>(width) / BitSize)),	// ����BitSize����1�Ƃ��ĕێ�
		_dataSizeHeight(height) {									// �c�͕ς��Ȃ��̂ł��̂܂�

		//�z���������
		Clear();
	}

	bool CellManager::SetCellLive(int x, int y, bool enable) {

		//���E�`�F�b�N
		if (!CheckBorder(x, y)) return false;

		int offset = GetByteOffset(x, y);
		uint_fast64_t mask = GetByteMask(x);

		if (enable) _cellData[offset] |= mask;
		else        _cellData[offset] &= ~mask;

		return true;
	}

	void CellManager::GenerationUpdate() {

		// �̂��ɃX���b�v����̂ŁA�]���Ɋm�ۂ��Ă���
		vector<uint_fast64_t> buff((_dataSizeWidth + 2) * (_dataSizeHeight + 2), 0ULL);

		// �]���Ɋm�ۂ��������炷
		for (int y = 1; y <= _dataSizeHeight; y++) {
			for (int x = 1; x <= _dataSizeWidth; x++) {

				// �z���̈ʒu���v�Z
				int offset = x + y * (_dataSizeWidth + 2);

				//1�u���b�N�X�V
				buff[offset] = NextGeneration(offset);
			}
		}

		//�X�V�𔽉f
		_cellData.swap(buff);
	}

	uint_fast64_t CellManager::NextGeneration(int offset) {
	
		// �������̂��߁A�r�b�g�{�[�h�ōs�� //

		// a~h�����Z���āA�r�b�g���Ƃ�1�̐������߂�
		// ���C�t�Q�[���ł́A�r�b�g���Ƃ̐���2�̂Ƃ���3�̂Ƃ��̂��̂����ł悢

		// �㕔
		auto upOffset = offset - (2 + _dataSizeWidth);
		auto up = _cellData[upOffset];

		// ����
		auto data = _cellData[offset];

		// ����
		auto downOffset = offset + (2 + _dataSizeWidth);
		auto down = _cellData[downOffset];

		// �ŏ�ʃr�b�g��ۑ����Ă���
		const auto MSB = 0x8000000000000000ULL;

		// ���ߖT�����߂�
		auto a = (up >> 1) | (!(_cellData[upOffset - 1] & 0x01) ? 0 : MSB);
		auto b = up;
		auto c = (up << 1) | (!(_cellData[upOffset + 1] & 0x80) ? 0 : 1);
		auto d = (data >> 1) | (!(_cellData[offset - 1] & 0x01) ? 0 : MSB);
		auto e = (data << 1) | (!(_cellData[offset + 1] & 0x80) ? 0 : 1);
		auto f = (down >> 1) | (!(_cellData[downOffset - 1] & 0x01) ? 0 : MSB);
		auto g = down;
		auto h = (down << 1) | (!(_cellData[downOffset - 1] & 0x80) ? 0 : 1);

		// ���v�l�𑁂����߂邽�߂ɁA�r�b�g���Z�ŋ��߂�
		auto xab = a & b;	//	a + b �̏�ʃr�b�g
		a ^= b;
		auto xcd = c & d;
		c ^= d;
		auto xef = e & f;
		e ^= f;
		auto xgh = g & h;
		g ^= h;

		// (xab a) + (xcd c) �� (c b a)		�_�����Z�񐔁F�T
		d = a & c;
		a ^= c;
		c = xab & xcd;		//	b2 ���P�ɂȂ�̂� (1 0) + (1 0) �̎��̂�
		b = xab ^ xcd ^ d;

		// (xef e) + (xgh g) �� (g f e)		�_�����Z�񐔁F�T
		h = e & g;
		e ^= g;
		g = xef & xgh;		//	b2 ���P�ɂȂ�̂� (1 0) + (1 0) �̎��̂�
		f = xef ^ xgh ^ h;

		//	(c b a) + (g f e) �� (c b a)		�_�����Z�񐔁F�X
		d = a & e;
		a ^= e;
		h = b & f;
		b ^= f;
		h |= b & d;			//	d �� b0 ����̌����
		b ^= d;
		c ^= g ^ h;

		//x ~ s3�܂� �_�����Z�񐔁F�T
		auto x = ~c & b;

		// a~h�̊e�r�b�g���Ƃ̍��v�l���v�Z
		// �� : s2�ɂ́A���v�l��2�̎��݂̂��̃r�b�g��On�ɂȂ�
		auto s2 = x & ~a;
		auto s3 = x & a;

		// ���C�t�Q�[���̐��������ɓ��Ă͂߂ĕԂ�
		return (data & s2) | s3;
	}
}