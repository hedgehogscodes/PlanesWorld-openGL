# Проект: PlanesWorld

Проект представляет собой сцену из дороги и летающими самолетами. Дорога и трава затекстурированы, при этом текстуры можно отключить. Также сцена освещена двумя источниками света (направленным и позиционным), которые можно включить и выключить по отдельности. Реализована возможность перемещения наблюдателя по сцене при помощи стрелок «вверх, вниз, вправо, влево», запуск и пауза анимации полета самолетов на кнопки M и N соответственно.

**Запуск приложения:**<br>
Для запуска приложения необходимо открыть файл PlanesWorld.exe

**Управление:**<br>
1. Стрелки «вверх, вниз, вправо, влево» - перемещение наблюдателя
2. J - включение/выключение текстур
3. K - включение/выключение источника направленного света
4. L - включение/выключение источника позиционного света
5. M - Запуск анимации полета самолетов 
6. N - Пауза анимации полета самолетов 
7. «+» - Увеличение скорости полета самолетов
8. «-» - Уменьшение скорости полета самолетов

**Технологии использованные в проекте:**<br>
1. OpenGL.

**Краткое описание модели**<br>
Модель представляет собой затекстурированные дороги, на которых расположены незатекстурированные полосы, затекстурированное поле и незатекстурированные самолеты.

Внешний вид модели представлен на рисунке 1.
 
<div>
<p>
  <img  src="https://github.com/hedgehogscodes/PlanesWorld-openGL/blob/main/assets/planes.png"/>
</p>
</div>
Рисунок 1 – Модель с позиции наблюдателя<br>

**Создание модели**<br>
Для отображения модели на экране необходимо проинициализировать значения. Для хранения характеристик самолетов объявим массив структур planes[].  В структуре будем хранить такие характеристики как скорость, компоненты red, green, blue, координаты x, y, z.
Далее мы будем инициализировать наш массив. Для инициализации создадим функцию add_plane(), в котором для каждого элемента массива будем генерировать цвета r, g, b. Скорость самолетам зададим 0.00019.
Для отображения самолетов будем использовать функцию drawPlane().
В данной функции будем рисовать самолеты в координатах x,y и z, зададим диффузное составляющее материала и цвет для каждой части самолета.
При расчете полета используются математические функции необходимые для вычисления положения и направления полета.
Для рисования плоскости будем использовать glBegin(GL_QUADS).
Создадим двойной цикл, первый будет отвечать за координату x, а второй за координату y. Итогом работы цикла будет поле состоящее из квадратов.

**Освещение сцены**<br>
После генерации модели необходимо настроить освещение. 
Первый источник света будет направленным. Хорошим примером источника направленного света может является Солнце. Оно будет находится в бесконечности и свет из него будет распространяться в заданном направлении.
Второй источник света имитирует фонарик над плоскостью и имеет тип "Прожектор" (позиционный источник). Цвет рассеянного источника света задается как белый. Его позиция (GL_POSITION) будет меняться вместе с положением камеры. Угол между осью и стороной конуса (GL_SPOT_CUTOFF) будет составлять 70 градусов.
Параметры источников освещения определяются в функции renderScene ().

**Текстурирование объектов модели**<br>
Текстура для нашей плоскости будет создаваться с помощью структуры data, в которой хранятся значения цветов текстуры. Функция glGenTextures() принимает в качестве первого аргумента количество текстур для генерации, а в качестве второго аргумента — массив GLuint в котором будут храниться идентификаторы этих текстур. Также мы привяжем его для того, чтобы функции, использующие текстуры, знали какую текстуру использовать: glBindTexture(GL_TEXTURE_2D, texture). После привязки текстуры мы можем начать генерировать данные текстуры, используя предварительно созданную структуру data: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data).
Создание текстур определяется в функцииях texturing () и texturing2() .
Для наложения текстуры на плоскость будем использовать glTexCoord2f(0, 0); в функции renderScene() для того, чтобы текстуры рисовались вместе с отрисовкой плоскости.
Перед применением текстуры разрешается использование текстур (GL_TEXTURE_2D) при помощи команды glEnable() если переменная texturetoggle = true. Перед непосредственным применением текстура устанавливается как активная.

**Перемещение наблюдателя по сцене**<br>
Для перемещения по сцене будем пользоваться функцией gluLookAt
Во-первых, нам нужны глобальные переменные для хранения параметров камеры. Переменные будут хранить как положение камеры, так и вектор, который дает нам цель направления. Мы также будем хранить угол поворота.
Следовательно,нам нужно:
float angle - Угол поворота вокруг оси "у". Эта переменная позволит поворачивать камеру.
- float x, z: переменные, хранящие положение камеры в плоскости "xz".
- float lx, lz: координаты вектора, определяющие направление перемещения камеры.
Функция gluLookAt обеспечивает простой и интуитивно понятный способ определения положения и ориентации камеры. В основном это три группы параметров, каждый из которых состоит из 3 значений с плавающей точкой. Первые три значения указывают положение камеры. Второй набор значений определяет точку зрения камеры. На самом деле это может быть любая точка в нашей линии обзора. Последняя группа параметров указывает на вектор, это, как правило, установленные значения ( 0.0, 1.0 , 0.0), это означает, что камера не наклонена. Итак, первый набор параметров х, у, и z - начальное положение камеры. Второй набор параметров, это направление вектора обзора, рассчитывается сложением вектора, который определяет нашу линию обзора с положением камеры. Точка = прямая видимость + координаты положения камеры. 
gluLookAt(   x, 1.0f,     z,
		  x+lx, 1.0f,  z+lz,
		  0.0f, 1.0f,  0.0f );
Теперь нужно создать функцию перемещения камеры, обрабатывающую нажатия клавиш перемещения. Мы используем клавиши "влево" и "вправо", чтобы повернуть камеру, или изменить вектор, который определяет линию обзора. Клавиши "вверх" и "вниз" используются для перемещения вдоль нынешней линии обзора. Когда пользователь нажимает левую или правую клавиши, переменная угла наклона изменяется соответствующим образом. На основании значения угла она вычисляет соответствующие значения для новых компонентов lx и lz линии вектора обзора. Новые значения lx и lz обображаются на единичном круге на плоскости "xz". Поэтому, учитывая переменную угла angle, новые значения для lx и lz, расчитываются по формуле:
lx = sin(ang);
lz = -cos(ang);
Мы также хотим перемещать камеру вдоль линии обзора. Для этого мы собираемся добавить или вычесть часть линии вектора обзора нашей текущей позиции, когда стрелки "вверх" и "вниз" будут нажаты соответсвенно. Например, для перемещения камеры вперед новые значения для переменных x и z будут:
x += deltaMove * lx * 0.1f;
z += deltaMove * lz * 0.1f;
deltaMove - возможная реализация скорости.
