#pragma once
#include <Windows.h>
#include <string>

#include "CellManager.h"

namespace LifeGameSystem {

	using namespace std;

	// �Z���̕`����s���N���X
	class Renderer final {

		const float BorderScale = 0.125f;	// �Z���̑傫���ɑ΂���g�̃T�C�Y��

		int _deadCellColor;					// �Z����Off�ɂȂ��Ă���Ƃ��̐F
		int _liveCellColor;					// �Z����On�ɂȂ��Ă���Ƃ��̐F

		int _viewWidth, _viewHeight;		// �o�͂�����ʂ̃T�C�Y

		int _cellPixelSize;					// 1�̃Z���̕`��T�C�Y
		int _borderPixelSize;				// �g���̕`��T�C�Y
		int _cellAreaPixelSize;				// �Z���Ƙg���𑫂����T�C�Y

		int _drawCellCountWidth;			// �`�悷��Z���̌� SetCellSize�Ŏ��O�Ɍv�Z�����
		int _drawCellCountHeight;

	public:
		Renderer(HWND);
		~Renderer();

		// �o�b�N�o�b�t�@�̎擾
		const void* GetBackBuffer() const;

		// �}�E�X�̈ʒu����Z���̍��W�����߂�
		void GetCellPosition(int mousePosX, int mousePosY, int& cellPosX, int& cellPosY) {

			cellPosX = mousePosX / _cellAreaPixelSize;
			cellPosY = mousePosY / _cellAreaPixelSize;
		}

		// �Z���̃T�C�Y��ݒ�
		void SetCellSize(int cellSize);

		// �`��
		void Draw(CellManager&) const;
	};

}