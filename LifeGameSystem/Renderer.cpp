#include "stdafx.h"
#include "Renderer.h"
#include "DxLib.h"

namespace LifeGameSystem {

	using namespace DxLib;

	Renderer::Renderer(HWND hwnd) : 
		_deadCellColor(GetColor(0, 0, 0)),		//��
		_liveCellColor(GetColor(0, 255, 0)) {	//��

		// Dx���C�u�����������ݒ�
		SetUseDirect3DVersion(DX_DIRECT3D_9EX);				// DirectX 9EX�ɂ���
		SetUserWindow(hwnd);								// �`��E�B���h�E�̐ݒ�
		ChangeWindowMode(TRUE);								// �E�B���h�E���[�h�ɐݒ�
		SetWindowSizeChangeEnableFlag(TRUE);				// �E�B���h�E�T�C�Y���ς�

		GetDefaultState(&_viewWidth, &_viewHeight, NULL);	//�E�B���h�E�T�C�Y���擾
		SetGraphMode(_viewWidth, _viewHeight, 32);			// �O���t�B�b�N���[�h�̐ݒ�(�ő�)

		SetAlwaysRunFlag(TRUE);								// ��A�N�e�B�u�����������s
		SetDrawScreen(DX_SCREEN_BACK);						// �`�����o�b�N�o�b�t�@�֐ݒ�
		SetUseFPUPreserveFlag(TRUE);						// FPU�̐��x�𗎂Ƃ��Ȃ�
		SetWaitVSyncFlag(FALSE);							// VSync�����𖳌�
		SetOutApplicationLogValidFlag(FALSE);				// ���O�o�͒�~
		SetDoubleStartValidFlag(TRUE);						// ���d�N��������
		SetUseIMEFlag(TRUE);								// IME��L��

		// Dx���C�u�����̏�����
		if (DxLib_Init() == -1) return;

		// �w�i�F�̐ݒ�
		SetBackgroundColor(32, 32, 32);

		// �Z���̃T�C�Y��ݒ�
		SetCellSize(16);

	}

	Renderer::~Renderer() {
		// Dx���C�u�����̏I������
		DxLib_End();
	}

	const void* Renderer::GetBackBuffer() const {
		// D3DImage�ɕ`�悷�邽�߂�
		// �o�b�N�o�b�t�@�ւ̃|�C���^���擾�ł���悤�ɂ���
		return GetUseDirect3D9BackBufferSurface();
	}

	void Renderer::SetCellSize(int cellSize) {

		_cellPixelSize = cellSize;
		//1�����̏ꍇ�͕`�悳��Ȃ��̂ŁA�{�[�_�[���X�ɂȂ�
		_borderPixelSize = BorderScale * cellSize;

		//�`�悷��Z���̎��O�v�Z
		_cellAreaPixelSize = _cellPixelSize + _borderPixelSize;
		_drawCellCountWidth = _viewWidth / _cellAreaPixelSize + 1;
		_drawCellCountHeight = _viewHeight / _cellAreaPixelSize + 1;
	}

	void Renderer::Draw(CellManager& cellManager) const {

		//��ʂ�����
		ClearDrawScreen();

		// �Z����`��
		for (int i = 0; i < _drawCellCountHeight; i++) {
			for (int j = 0; j < _drawCellCountWidth; j++) {

				auto startX = j * _cellAreaPixelSize;
				auto startY = i * _cellAreaPixelSize;

				auto color = cellManager.GetCellLive(j, i) ? _liveCellColor : _deadCellColor;

				DrawBox(startX, startY, startX + _cellPixelSize, startY + _cellPixelSize, color, true);
			}
		}

		ScreenFlip();
	}

}