using System.Windows;
using LifeGameSystem;

namespace LifeGameSimulator.Tools {

	/// <summary>
	/// セルを描画するツール
	/// </summary>
	class PenTool : ITool {

		private bool _isClick; 

		public void OnMouseDown(LifeGameModel lifeGameModel, Point mousePosition) {
			_isClick = true;
			lifeGameModel.SetCellLive((int)mousePosition.X, (int)mousePosition.Y, true);
		}

		public void OnMouseMove(LifeGameModel lifeGameModel, Point mousePosition) {
			if(!_isClick) return;
			lifeGameModel.SetCellLive((int)mousePosition.X, (int)mousePosition.Y, true);

		}

		public void OnMouseUp(LifeGameModel lifeGameModel, Point mousePosition) {
			_isClick = false;
		}
	}
}
