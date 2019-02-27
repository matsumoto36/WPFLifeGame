#pragma once

#include <vector>

namespace LifeGameSystem {

	using namespace std;

	// セルの情報を管理するクラス
	class CellManager {

		// 型のビット数
		const size_t BitSize;

		// データサイズ(データの個数)
		int _dataSizeWidth;
		int _dataSizeHeight;

		// セルのデータ
		vector<uint_fast64_t> _cellData;

		// x,yの位置からcellDataの配列の位置を取得
		size_t GetByteOffset(int x, int y) {
			// 余分に確保された分を調整して返却
			return x / BitSize + 1 + (y + 1) * (_dataSizeWidth + 2);
		}

		// xの位置のマスクを返す
		uint_fast64_t GetByteMask(int x) {
			return 0x8000000000000000ULL >> (x & (BitSize - 1));
		}

		// 配列の位置の一世代先を求める
		uint_fast64_t NextGeneration(int);

	public:
		CellManager(int width, int height);
		~CellManager() {}

		// セルを初期化する
		void Clear() {

			// 初期化
			// 次世代計算時の境界チェックをなくすために、上下左右余分に確保しておく
			_cellData = vector<uint_fast64_t>((_dataSizeWidth + 2) * (_dataSizeHeight + 2), 0ULL);
		}

		// セルの横の個数を取得
		int GetCellSizeWidth() const { return _dataSizeWidth * BitSize; }
		// セルの縦の個数を取得
		int GetCellSizeHeight() const { return _dataSizeHeight; }

		//範囲内かどうか調べる
		bool CheckBorder(int x, int y) {
			return
				// 実際に確保されたセル情報にはアクセスしないので、
				// オフセット等は気にしなくてもよい
				!(x < 0 || x > _dataSizeWidth * BitSize) && 
				!(y < 0 || x > _dataSizeHeight);
		}

		// セルを取得
		bool GetCellLive(int x, int y) {
			if (!CheckBorder(x, y)) return false;
			return (_cellData[GetByteOffset(x, y)] & GetByteMask(x)) != 0;
		}

		// セルの状態をセット
		bool SetCellLive(int x, int y, bool isLive);

		// 1世代更新
		void GenerationUpdate();
	};

}