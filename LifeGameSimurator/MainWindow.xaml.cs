using System;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Interop;
using LifeGameSystem;
using System.Diagnostics;

namespace LifeGameSimulator {
	/// <summary>
	/// MainWindow.xaml の相互作用ロジック
	/// </summary>
	public partial class MainWindow : Window {

		private const int NormalWaitMilliSec = 1000 / 30;	// セルの更新待機時間
		private const int FastWaitMilliSec = 0;				// 高速状態時

		private LifeGameModel _lifeGameModel;				// ライフゲームを制御する
		private D3DImage _d3DImage;							// Dxライブラリの描画を表示する

		private readonly ITool[] _mouseTool;				// マウスの操作に対応したツール

		private bool _isExecuted;							// ボタンの状態を保存
		private bool _isExecuteFastMode;

		public MainWindow() {
			InitializeComponent();

			// ツールを設定
			_mouseTool = new ITool[5];
			_mouseTool[(int)MouseButton.Left] = new Tools.PenTool();			// 左クリックにペンツールを指定
			_mouseTool[(int)MouseButton.Right] = new Tools.EraseTool();			// 左クリックに消しゴムツールを指定

		}

		private void DrawImage_Loaded(object sender, RoutedEventArgs e) {

			// ウィンドウハンドルを生成
			var hwndSrc = new HwndSource(0, 0, 0, 0, 0, "DxLib", IntPtr.Zero);

			_lifeGameModel = new LifeGameModel(hwndSrc.Handle, 128, 128);
			_d3DImage = new D3DImage();

			// バックバッファの設定
			SetBackBuffer();
			DrawImage.Source = _d3DImage;
			
			// 描画の更新
			CompositionTarget.Rendering += CompositionTarget_Rendering;
			_d3DImage.IsFrontBufferAvailableChanged += D3DImage_IsFrontBufferAvailableChanged;
		}

		private void DrawImage_MouseDown(object sender, MouseButtonEventArgs e) {

			// DrawImage内のマウス座標を取得
			var point = e.GetPosition(DrawImage);

			// 対応したツールの操作を実行
			_mouseTool[(int)e.ChangedButton]?.OnMouseDown(_lifeGameModel, point);

		}

		private void DrawImage_MouseMove(object sender, MouseEventArgs e) {

			// DrawImage内のマウス座標を取得
			var point = e.GetPosition(DrawImage);

			// 対応したツールの操作を実行
			foreach(var item in _mouseTool) {
				item?.OnMouseMove(_lifeGameModel, point);
			}
		}

		private void DrawImage_MouseUp(object sender, MouseButtonEventArgs e) {

			// DrawImage内のマウス座標を取得
			var point = e.GetPosition(DrawImage);

			// 対応したツールの操作を実行
			_mouseTool[(int)e.ChangedButton]?.OnMouseUp(_lifeGameModel, point);

		}

		private void ExitButton_Click(object sender, RoutedEventArgs e) {
			// 終了
			Close();
		}

		private void ExecuteButton_Click(object sender, RoutedEventArgs e) {

			//実行とストップを反転する
			_isExecuted = !_isExecuted;
			ExecuteButton.IsChecked = _isExecuted;
			if(_isExecuted) {
				_lifeGameModel.Start(_isExecuteFastMode ? FastWaitMilliSec : NormalWaitMilliSec);
			}
			else {
				_lifeGameModel.Stop();
			}
		}

		private void StepButton_Click(object sender, RoutedEventArgs e) {

			if(_isExecuted) {
				ExecuteButton.IsChecked = false;
				_isExecuted = false;
				_lifeGameModel.Stop();
			}

			// 世代を更新
			// 描画は別のコントロール経由で更新される
			_lifeGameModel.Next();
		}
		private void FastExecuteButton_Click(object sender, RoutedEventArgs e) {

			_isExecuteFastMode = !_isExecuteFastMode;

			var waitTime = _isExecuteFastMode ? FastWaitMilliSec : NormalWaitMilliSec;

			// モード変更時は強制的に開始
			if(_isExecuted) {
				_lifeGameModel.ReStart(waitTime);
			}
			else {
				ExecuteButton.IsChecked = true;
				_isExecuted = true;
				_lifeGameModel.Start(waitTime);
			}
		}

		/// <summary>
		/// 表示を更新する
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void CompositionTarget_Rendering(object sender, EventArgs e) {

			try {
				if(_d3DImage.IsFrontBufferAvailable) {

					_d3DImage.Lock();
					_lifeGameModel.Draw();
					_d3DImage.AddDirtyRect(new Int32Rect(0, 0, _d3DImage.PixelWidth, _d3DImage.PixelHeight));
					_d3DImage.Unlock();
				}
			}
			catch(Exception ex) {
				Trace.WriteLine(ex.ToString());
			}

			//世代の表示を更新
			StatusLabel.Content = $"Generation {_lifeGameModel.GetGeneration()}";
		}

		/// <summary>
		/// フロントバッファの更新
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void D3DImage_IsFrontBufferAvailableChanged(object sender, DependencyPropertyChangedEventArgs e) {

			if(_d3DImage.IsFrontBufferAvailable) {
				CompositionTarget.Rendering += CompositionTarget_Rendering;
			}
			else {
				CompositionTarget.Rendering -= CompositionTarget_Rendering;
			}
		}

		/// <summary>
		/// バックバッファの設定
		/// </summary>
		private void SetBackBuffer() {
			_d3DImage.Lock();
			_d3DImage.SetBackBuffer(D3DResourceType.IDirect3DSurface9, _lifeGameModel.GetBackBuffer());
			_d3DImage.Unlock();
		}
	}
}
