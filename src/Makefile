CC = g++
CFLAGS = -Werror -Wall -Wextra -std=c++17 -fPIC
GTEST_FLAGS = -lgtest -lgtest_main -pthread
QT = qmake

MODEL_DIR = ./models
VIEW_DIR = ./view
CONTROLLER_DIR = ./controller
TEST_DIR = ./tests
MODELS_CLANG = $(MODEL_DIR)/glwidget.cpp $(MODEL_DIR)/parserModel.cpp $(MODEL_DIR)/parserModel.h

LIB_PARSER_NAME = libparsermodel.a
LIB_CONTROLLER_NAME = libcontroller.a

TEST_BIN_NAME = test_executable
QT_BIN_NAME = qt_app

QT_INCPATH = -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets -I/usr/include/x86_64-linux-gnu/qt5/QtGui -I/usr/include/x86_64-linux-gnu/qt5/QtCore -I/usr/include/x86_64-linux-gnu/qt5 -I/usr/include/x86_64-linux-gnu/qt5/QtCharts
QT_LIBS = -lQt5Widgets -lQt5Gui -lQt5Core -lQt5Charts -lGL -lGLU

all: coverage

libs: clean parser_lib controller_lib

gcov_flag:
	$(eval CFLAGS += -fprofile-arcs -ftest-coverage)

test: gcov_flag libs
	$(CC) $(CFLAGS) $(TEST_DIR)/*.cpp -L. -lparsermodel $(GTEST_FLAGS) -o $(TEST_BIN_NAME)
	./$(TEST_BIN_NAME)

coverage: test
	lcov --directory . --capture --output-file coverage.info
	lcov --remove coverage.info '/usr/*' '*/gtest/*' --output-file coverage.info.cleaned
	genhtml coverage.info.cleaned --output-directory coverage_report


parser_lib:
	$(CC) $(CFLAGS) -c $(MODEL_DIR)/parserModel.cpp -o $(MODEL_DIR)/parserModel.o
	ar rcs $(LIB_PARSER_NAME) $(MODEL_DIR)/parserModel.o
	rm -rf models/*.o

controller_lib: parser_lib
	$(CC) $(CFLAGS) $(QT_INCPATH) -c $(CONTROLLER_DIR)/*.cpp -I$(CONTROLLER_DIR) -I$(MODEL_DIR) -I$(VIEW_DIR)
	ar rcs $(LIB_CONTROLLER_NAME) controller.o
	ar -M <create_lib_controller.mri
	rm -f *.o

lint:
	clang-format -n $(VIEW_DIR)/*.cpp $(VIEW_DIR)/*.h $(CONTROLLER_DIR)/*.cpp $(CONTROLLER_DIR)/*.h $(TEST_DIR)/*.cpp $(MODELS_CLANG)

fix_lint:
	clang-format -i $(VIEW_DIR)/*.cpp $(VIEW_DIR)/*.h $(CONTROLLER_DIR)/*.cpp $(CONTROLLER_DIR)/*.h $(TEST_DIR)/*.cpp $(MODELS_CLANG)

install: libs
	cd $(VIEW_DIR) && $(QT) view.pro -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug && make
	mv view/view ./$(QT_BIN_NAME)
	./$(QT_BIN_NAME)

uninstall: clean
	rm -f $(QT_BIN_NAME)

dvi:
	open ../README.md

dist: install
	tar -czvf qt_app.tar.gz $(QT_BIN_NAME) view

clean:
	rm -f *.o *.a $(TEST_BIN_NAME) $(QT_BIN_NAME) *.gcda *.gcno *.gcov coverage.info
	if [ -d $(VIEW_DIR) ]; then cd $(VIEW_DIR) && $(QT) view.pro -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug && make clean || true; fi
	rm -rf $(VIEW_DIR)/Makefile $(VIEW_DIR)/Makefile.Debug $(VIEW_DIR)/Makefile.Release $(VIEW_DIR)/view.pro.user $(VIEW_DIR)/view.pro.user
	rm -f settings.json
	rm -f *.cleaned
	rm -rf $(MODEL_DIR)/*.gcda $(MODEL_DIR)/*.gcno $(MODEL_DIR)/*.gcov
	rm -rf coverage_report
git:
	sh git.sh