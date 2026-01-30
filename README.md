# Sheep Invaders trên STM32F429

Sheep Invaders là một trò chơi lấy cảm hứng từ trò chơi Space Invaders, được lập trình trên vi điều khiển STM32F429ZIT6, Game theo thể loại endless giúp kích thích sự tranh đấu của người chơi, đem tới sự dễ tiếp cận, hướng tới sự đơn giản những vẫn cuốn hút như tựa game Flappy Bird.

## Tính năng nổi bật

- Game cho phép người chơi điều khiển nhân vật người lính di chuyển sang trái, phải và bắn đạn để tiêu diệt các con cừu đang di chuyển từ trên xuống. 
- Về lập trình, game sử dụng Object Pooling để quản lý hình ảnh, phù hợp với bộ nhớ RAM ít ỏi của vi điều khiển. Với tính năng quan trọng như xử lí va chạm, nhóm đã tối ưu bằng cách sử dụng thuật toán intersect của TouchGFX kết hợp với lọc Lane để giảm tải cho CPU. 
- Đồng thời, nhóm cũng sử dụng bộ tạo số ngẫu nhiên phần cứng (RNG) để đảm bảo cừu xuất hiện thật sự ngẫu nhiên, thay vì dùng các hàm pseudo random thông thường, vốn có điểm yếu là các giá trị không thực sự ngẫu nhiên mà vẫn tuân theo một chu kì nào đó
- Về phần cứng, nhóm cho phép người chơi sử dụng hệ thống button kết nối với vi điều khiển, giúp người chơi thuận tiện hơn. Sử dụng hai biến theo dõi trạng thái ở hai thời điểm liền kề của chân GPIO giúp phát hiện xung falling edge một cách đơn giản và nhanh nhạy, giúp đáp ứng trải nghiệm của người chơi
- Nhóm cũng sử dụng loa buzzer để kết nối âm thanh cho trò chơi, tạo cảm giác sống động hơn khi người chơi chơi game
- Trò chơi cũng cho phép lưu Highscore trong một phiên chơi. Ngay khi người chơi vượt mốc, điểm số Highscore sẽ luôn được cập nhật. Ngoài ra còn có các button cảm ứng để Pause Game và bắt đầu Game mới. 

## Kiến trúc phần mềm

Game được xây dựng theo mô hình MVP (Model-View-Presenter) giúp tách biệt rõ ràng giữa logic và giao diện:

- Model.cpp: Đây là nơi xử lý logic 
    - tick(): Kiểm tra trạng thái các chân GPIO (PE2, PE3, PC3). Nếu phát hiện nút được nhấn (xung xuống), nó sẽ báo cho Presenter. Ngoài ra còn quản lý bộ đếm để tạo tiếng bíp không gây treo máy.
    - GetRandomNumber(): Gọi trực tiếp hàm của phần cứng STM32 để lấy một số ngẫu nhiên dùng cho việc xuất hiện cừu.
    - saveHighScore(): So sánh điểm hiện tại với điểm cao nhất đã lưu, nếu phá kỷ lục thì cập nhật lại.
- Screen1View.cpp: Đây là nơi quản lý  màn hình. Hàm handleTickEvent() sẽ lấy dữ liệu từ Model để cập nhật vị trí các con cừu và đạn, đảm bảo hình ảnh hiển thị mượt mà. 
    - handleTickEvent(): Vòng lặp chính của giao diện. Nó lo việc cho cừu rơi xuống, di chuyển đạn, và kiểm tra xem đạn có trúng cừu hay cừu có đâm vào người chơi không.
    - updateBulletPosition(): Cứ mỗi 1/60 giây lại trừ tọa độ Y của đạn để nó bay lên trên.
    - setupScreen(): Chạy duy nhất một lần khi mở game để sắp xếp các con cừu và đạn vào "hàng chờ" (Object Pool) sẵn sàng sử dụng.
- Screen1Presenter.cpp: Đóng vai trò là cầu nối. 
    - activate(): Chuẩn bị dữ liệu và trạng thái ban đầu trước khi màn hình hiện ra.
    - updateScore(): Lấy điểm số từ Model rồi gọi hàm hiển thị của View để người chơi thấy điểm tăng lên.

## Hạn chế và hướng phát triển trong tương lai
- Còn thiếu chuyên nghiệp trong quản lí dự án. Việc chỉ tập trung nhóm làm một chỗ đã giới hạn rất nhiều khả năng phát triển dự án hoàn thiện hơn, làm lãng phí thời gian, dễ thỏa thuận, thiếu sự phản biện để nâng cao chất lượng sản phẩm
- Chưa tận dụng hết sức mạnh của vi xử lí, chỉ sử dụng đơn giản 6 object cừu để pooling lần lượt, cũng như dùng duy nhất 1 ScreenView để hiển thị toàn bộ trò chơi. Vì vậy, game thiếu đi sự hấp dẫn, đúng ra có thể phát triển nhiều tính năng như level game (thay đổi hướng đi của cừu, thay đổi tốc độ đạn,..) thêm các dạng đạn đặc biệt tăng sức mạnh cho nhân vật, hay thêm các ràng buộc để kết thúc game (như không cho phép cừu chạm xuống đáy màn hình,...)
- Về phần lập trình, còn đang đơn giản, vai trò của các class và hàn chồng chéo lên nhau, như model vẫn nhận xử lí tín hiệu ngoại vi, lại cũng điều khiển cả logic kiểm tra va chạm để điều khiển hiển thị. Trong tương lai, có thể phát triển dự án chuyên nghiệp hơn, áp dụng các design pattern cũng như các nguyên tắc của OOP để code có chất lượng tốt hơn, sạch hơn, dễ bảo trì và mở rộng


## Thành viên
- Nguyễn Thành Tùng – 20226071
- Trịnh Lương Việt – 20235967
- Lê Quốc Tuấn – 20236008

