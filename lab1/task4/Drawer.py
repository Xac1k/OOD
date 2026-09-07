#!/usr/bin/env python3
"""
Генератор команд для DRAWER CLI
Создает пейзаж: деревья слева, река справа
"""

def generate_landscape_commands():
    """Генерирует команды для рисования пейзажа"""

    commands = []

    # ===== НАСТРОЙКИ =====
    canvas_width = 800
    canvas_height = 600

    # Цвета
    SKY_BLUE = "#87CEEB"
    GRASS_GREEN = "#4CAF50"
    DARK_GREEN = "#2E7D32"
    TREE_TRUNK = "#8D6E63"
    LEAF_GREEN = "#388E3C"
    LEAF_LIGHT = "#66BB6A"
    RIVER_BLUE = "#2196F3"
    RIVER_LIGHT = "#64B5F6"
    SUN_YELLOW = "#FFD54F"
    CLOUD_WHITE = "#FFFFFF"
    FLOWER_RED = "#E53935"
    FLOWER_YELLOW = "#FFEB3B"
    FLOWER_PINK = "#EC407A"

    # ===== ФОН =====
    # Небо (прямоугольник на весь холст)
    commands.append(f"AddShape sky {SKY_BLUE} rectangle 0 0 {canvas_width} {canvas_height}")

    # Солнце
    commands.append(f"AddShape sun {SUN_YELLOW} circle 680 80 45")
    commands.append(f"AddShape sun_glow {SUN_YELLOW} circle 680 80 55")

    # Облака
    commands.append(f"AddShape cloud1 {CLOUD_WHITE} circle 150 80 35")
    commands.append(f"AddShape cloud2 {CLOUD_WHITE} circle 190 70 40")
    commands.append(f"AddShape cloud3 {CLOUD_WHITE} circle 230 80 30")
    commands.append(f"AddShape cloud4 {CLOUD_WHITE} circle 170 100 25")

    commands.append(f"AddShape cloud5 {CLOUD_WHITE} circle 450 60 30")
    commands.append(f"AddShape cloud6 {CLOUD_WHITE} circle 490 50 35")
    commands.append(f"AddShape cloud7 {CLOUD_WHITE} circle 530 60 28")

    # Земля (трава)
    commands.append(f"AddShape ground {GRASS_GREEN} rectangle 0 350 {canvas_width} 250")

    # ===== РЕКА (справа) =====
    # Основное русло реки
    commands.append(f"AddShape river1 {RIVER_BLUE} rectangle 550 350 250 250")

    # Изгибы реки (овалы, имитирующие течение)
    commands.append(f"AddShape river_curve1 {RIVER_LIGHT} circle 580 400 50")
    commands.append(f"AddShape river_curve2 {RIVER_LIGHT} circle 650 450 45")
    commands.append(f"AddShape river_curve3 {RIVER_BLUE} circle 720 500 40")
    commands.append(f"AddShape river_curve4 {RIVER_LIGHT} circle 600 520 35")

    # Блики на воде
    commands.append(f"AddShape water_highlight1 {RIVER_LIGHT} line 570 380 600 395")
    commands.append(f"AddShape water_highlight2 {RIVER_LIGHT} line 640 430 670 445")
    commands.append(f"AddShape water_highlight3 {RIVER_LIGHT} line 700 480 730 495")

    # Береговая линия (песок/земля у реки)
    commands.append(f"AddShape bank1 #A1887F rectangle 530 350 20 250")
    commands.append(f"AddShape bank2 #A1887F rectangle 770 350 20 250")

    # ===== ДЕРЕВЬЯ (слева) =====

    # Дерево 1 (большое, слева вверху)
    # Ствол
    commands.append(f"AddShape tree1_trunk {TREE_TRUNK} rectangle 80 300 20 80")
    # Крона (составная)
    commands.append(f"AddShape tree1_leaf1 {LEAF_GREEN} circle 70 280 45")
    commands.append(f"AddShape tree1_leaf2 {LEAF_LIGHT} circle 100 270 40")
    commands.append(f"AddShape tree1_leaf3 {DARK_GREEN} circle 85 260 50")
    commands.append(f"AddShape tree1_leaf4 {LEAF_GREEN} circle 60 290 30")
    commands.append(f"AddShape tree1_leaf5 {LEAF_LIGHT} circle 105 290 30")

    # Дерево 2 (среднее)
    commands.append(f"AddShape tree2_trunk {TREE_TRUNK} rectangle 200 320 15 60")
    commands.append(f"AddShape tree2_leaf1 {DARK_GREEN} circle 195 295 35")
    commands.append(f"AddShape tree2_leaf2 {LEAF_GREEN} circle 215 285 38")
    commands.append(f"AddShape tree2_leaf3 {LEAF_LIGHT} circle 205 275 40")

    # Дерево 3 (маленькое, ближе к центру)
    commands.append(f"AddShape tree3_trunk {TREE_TRUNK} rectangle 320 340 12 40")
    commands.append(f"AddShape tree3_leaf1 {LEAF_GREEN} circle 312 320 28")
    commands.append(f"AddShape tree3_leaf2 {DARK_GREEN} circle 328 310 30")
    commands.append(f"AddShape tree3_leaf3 {LEAF_LIGHT} circle 320 305 32")

    # Дерево 4 (ёлкообразное, слева снизу)
    commands.append(f"AddShape tree4_trunk {TREE_TRUNK} rectangle 50 340 10 40")
    commands.append(f"AddShape tree4_t1 {DARK_GREEN} triangle 35 310 55 310 45 280")
    commands.append(f"AddShape tree4_t2 {LEAF_GREEN} triangle 30 330 60 330 45 295")
    commands.append(f"AddShape tree4_t3 {LEAF_LIGHT} triangle 40 300 50 300 45 270")

    # Дерево 5 (справа от реки, на другом берегу)
    commands.append(f"AddShape tree5_trunk {TREE_TRUNK} rectangle 780 340 12 40")
    commands.append(f"AddShape tree5_leaf1 {LEAF_GREEN} circle 775 320 25")
    commands.append(f"AddShape tree5_leaf2 {DARK_GREEN} circle 790 310 28")
    commands.append(f"AddShape tree5_leaf3 {LEAF_LIGHT} circle 782 305 30")

    # ===== ЦВЕТЫ =====
    # Поляна с цветами (слева)
    commands.append(f"AddShape flower1 {FLOWER_RED} circle 150 400 5")
    commands.append(f"AddShape flower2 {FLOWER_YELLOW} circle 170 410 4")
    commands.append(f"AddShape flower3 {FLOWER_PINK} circle 135 420 5")
    commands.append(f"AddShape flower4 {FLOWER_RED} circle 180 390 4")
    commands.append(f"AddShape flower5 {FLOWER_YELLOW} circle 160 430 5")
    commands.append(f"AddShape flower6 {FLOWER_PINK} circle 190 420 4")
    commands.append(f"AddShape flower7 {FLOWER_RED} circle 145 440 5")
    commands.append(f"AddShape flower8 {FLOWER_YELLOW} circle 175 445 4")

    # Цветы на берегу реки
    commands.append(f"AddShape flower9 {FLOWER_PINK} circle 540 380 4")
    commands.append(f"AddShape flower10 {FLOWER_RED} circle 555 390 5")
    commands.append(f"AddShape flower11 {FLOWER_YELLOW} circle 530 400 4")

    # ===== ТРАВА (отдельные травинки) =====
    commands.append(f"AddShape grass1 {DARK_GREEN} line 250 380 255 365")
    commands.append(f"AddShape grass2 {DARK_GREEN} line 260 385 268 368")
    commands.append(f"AddShape grass3 {LEAF_GREEN} line 270 390 278 372")
    commands.append(f"AddShape grass4 {DARK_GREEN} line 290 395 295 378")
    commands.append(f"AddShape grass5 {LEAF_GREEN} line 300 400 308 382")

    # ===== КУСТЫ =====
    commands.append(f"AddShape bush1 {DARK_GREEN} circle 380 370 20")
    commands.append(f"AddShape bush2 {LEAF_GREEN} circle 400 365 22")
    commands.append(f"AddShape bush3 {LEAF_LIGHT} circle 390 360 18")

    commands.append(f"AddShape bush4 {DARK_GREEN} circle 460 380 15")
    commands.append(f"AddShape bush5 {LEAF_GREEN} circle 475 375 17")

    return commands


def generate_test_script():
    """Создает тестовый скрипт для DRAWER CLI"""

    print("=" * 60)
    print("ПЕЙЗАЖ: ДЕРЕВЬЯ СЛЕВА, РЕЧКА СПРАВА")
    print("=" * 60)
    print("\nСкопируйте и вставьте следующие команды в DRAWER CLI:")
    print("-" * 60)

    commands = generate_landscape_commands()

    # Выводим команды
    for cmd in commands:
        print(cmd)

    # Команды для сохранения
    print("\n" + "-" * 60)
    print("\n# Сохраните результат:")
    print("Save svg landscape.svg")
    print("Save png landscape.png")
    print("\n# Или нарисуйте всё сразу:")
    print("DrawPicture")
    print("\n" + "=" * 60)


def save_commands_to_file(filename="landscape_commands.txt"):
    """Сохраняет команды в файл"""
    commands = generate_landscape_commands()

    with open(filename, "w") as f:
        f.write("# Пейзаж: деревья слева, речка справа\n")
        f.write("# Сгенерировано для DRAWER CLI v1.0\n\n")

        for cmd in commands:
            f.write(cmd + "\n")

        f.write("\n# Сохранить изображение\n")
        f.write("Save svg landscape.svg\n")
        f.write("Save png landscape.png\n")
        f.write("\n# Нарисовать всё\n")
        f.write("DrawPicture\n")

    print(f"✅ Команды сохранены в файл: {filename}")
    print(f"   Запустите DRAWER CLI и выполните: source {filename}")
    print(f"   Или скопируйте содержимое файла и вставьте в DRAWER CLI")


if __name__ == "__main__":
    import sys

    if len(sys.argv) > 1 and sys.argv[1] == "--save":
        save_commands_to_file()
    else:
        generate_test_script()
        print("\n💡 Для сохранения команд в файл запустите:")
        print("   python3 landscape_generator.py --save")