#pragma once

#include <Windows.h>
#include <string>
#include <functional>

#include "Renderer.h"
#include "CellManager.h"

namespace LifeGameSystem {

	using namespace std;

	// ���C�t�Q�[���ōs�������̋��n��������
	class LifeGameSystem {

		unique_ptr<Renderer> _renderer;			// �`��S��
		unique_ptr<CellManager> _cellManager;	// �Z���̊Ǘ��S��

		unsigned int _generation;				// �i�񂾐���
		bool _isStarted;						// ������X�V���Ă��邩
		bool _canStartLoop;						// ���C�����[�v���J�n�ł��邩

		function<void()> _onStoppedLoopOnce;	// ���[�v�����S�Ɏ~�܂�����Ɏ��s�����B���s��͋����I��null�ɂȂ�

	public:
		LifeGameSystem(HWND hwnd, int mapSizeWidth, int mapSizeHeight) :
			_renderer(new Renderer(hwnd)),
			_cellManager(new CellManager(mapSizeWidth, mapSizeHeight)),
			_generation(0),
			_isStarted(false),
			_canStartLoop(true),
			_onStoppedLoopOnce(nullptr) {

		}

		// ���݂̐��㐔���擾
		const auto GetGeneration() const {
			return _generation;
		}

		// �o�b�N�o�b�t�@�̎擾
		const auto GetBackBuffer() const {
			return _renderer->GetBackBuffer();
		}

		// �}�E�X�̍��W����A�Z���̏�Ԃ�ύX����
		void SetCellLive(int clickPosX, int clickPosY, bool isLive) const;

		// �Z��������������
		void ClearCell() {
			_generation = 0;
			_cellManager->Clear();
		}

		// �w�肵���҂����ԂŃ��C�t�Q�[���̏������J�n����
		void Start(int);

		// ���C�t�Q�[���̏������~�߂�
		bool Stop() {
			if (_canStartLoop) return false;

			_generation = 0;
			_isStarted = false;

			return true;
		}

		// ���C�t�Q�[���̏������~�߂�
		bool Stop(function<void()> onCompleted) {
			if (!Stop()) return false;
			_onStoppedLoopOnce = onCompleted;
			return true;
		}

		// ���C�t�Q�[���̏������~�߂čēx�J�n����
		void ReStart(int milliSec) {
			if (_canStartLoop) {
				// �~�߂鏈�����K�v�Ȃ��ꍇ�͂��̂܂܊J�n
				Start(milliSec);
			}
			else {
				// �~�߂���Ƀ��X�^�[�g����
				Stop([this, milliSec]() {
					Start(milliSec);
				});
			}
		}

		// �����1�i�߂�
		void Next() {
			_cellManager->GenerationUpdate();
			_generation++;
		}

		// �`����s��
		void Draw() const {
			_renderer->Draw(*_cellManager);
		}

	private:

		// ���C�����[�v
		// �ʃX���b�h�ň������Ƃ�z��
		void LoopAsync(int, function<void()> callBack);
	};

}