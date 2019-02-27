#pragma once

#include <vector>

namespace LifeGameSystem {

	using namespace std;

	// �Z���̏����Ǘ�����N���X
	class CellManager {

		// �^�̃r�b�g��
		const size_t BitSize;

		// �f�[�^�T�C�Y(�f�[�^�̌�)
		int _dataSizeWidth;
		int _dataSizeHeight;

		// �Z���̃f�[�^
		vector<uint_fast64_t> _cellData;

		// x,y�̈ʒu����cellData�̔z��̈ʒu���擾
		size_t GetByteOffset(int x, int y) {
			// �]���Ɋm�ۂ��ꂽ���𒲐����ĕԋp
			return x / BitSize + 1 + (y + 1) * (_dataSizeWidth + 2);
		}

		// x�̈ʒu�̃}�X�N��Ԃ�
		uint_fast64_t GetByteMask(int x) {
			return 0x8000000000000000ULL >> (x & (BitSize - 1));
		}

		// �z��̈ʒu�̈ꐢ�������߂�
		uint_fast64_t NextGeneration(int);

	public:
		CellManager(int width, int height);
		~CellManager() {}

		// �Z��������������
		void Clear() {

			// ������
			// ������v�Z���̋��E�`�F�b�N���Ȃ������߂ɁA�㉺���E�]���Ɋm�ۂ��Ă���
			_cellData = vector<uint_fast64_t>((_dataSizeWidth + 2) * (_dataSizeHeight + 2), 0ULL);
		}

		// �Z���̉��̌����擾
		int GetCellSizeWidth() const { return _dataSizeWidth * BitSize; }
		// �Z���̏c�̌����擾
		int GetCellSizeHeight() const { return _dataSizeHeight; }

		//�͈͓����ǂ������ׂ�
		bool CheckBorder(int x, int y) {
			return
				// ���ۂɊm�ۂ��ꂽ�Z�����ɂ̓A�N�Z�X���Ȃ��̂ŁA
				// �I�t�Z�b�g���͋C�ɂ��Ȃ��Ă��悢
				!(x < 0 || x > _dataSizeWidth * BitSize) && 
				!(y < 0 || x > _dataSizeHeight);
		}

		// �Z�����擾
		bool GetCellLive(int x, int y) {
			if (!CheckBorder(x, y)) return false;
			return (_cellData[GetByteOffset(x, y)] & GetByteMask(x)) != 0;
		}

		// �Z���̏�Ԃ��Z�b�g
		bool SetCellLive(int x, int y, bool isLive);

		// 1����X�V
		void GenerationUpdate();
	};

}