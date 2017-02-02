cmake_minimum_required(VERSION 2.8)

project( PhotoMosaic )

find_package( OpenCV REQUIRED )

add_executable(main src/main.cpp)
add_executable(createDataset src/createDataset.cpp)
add_executable(photoMosaicAverageRGB src/photoMosaicAverageRGB.cpp)
add_executable(photoMosaicAverageRGBFaceDetector src/photoMosaicAverageRGBFaceDetector.cpp)

target_link_libraries(main ${OpenCV_LIBS})
target_link_libraries(createDataset ${OpenCV_LIBS})
target_link_libraries(photoMosaicAverageRGB ${OpenCV_LIBS})
target_link_libraries(photoMosaicAverageRGBFaceDetector ${OpenCV_LIBS})