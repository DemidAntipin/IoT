# Описание

* Устройство управляет матрицей светодиодов 2 на 2 с помощью 4 пинов.
* Режим свечения меняется каждые 2 секунды.
* Порядок режимов указан в массиве states (*ST* - 1-ый, левый верхний; *ND* - 2-ой, правый верхний, *RD* - 3-ий, левый нижний, *TH* - 4-ый, правый нижний). Считается, что плата при такой нумерации светодиодов расположена внизу по центру.

# Демонстрация

* Увидеть работу устройства можно по [ссылке](https://drive.google.com/file/d/1dBzoud4gmU3jeRDhpaXatUDRUFi9ownr/view?usp=sharing)

# Матрица режимов

<table>
  <tr>
    <th>leds</th>
    <th>D7</th>
    <th>D8</th>
    <th>D11</th>
    <th>D12</th>
  </tr>
  <tr>
    <td>1</td>
    <td>H</td>
    <td>L</td>
    <td>L</td>
    <td>H</td>
  </tr>
  <tr>
    <td>2</td>
    <td>H</td>
    <td>L</td>
    <td>H</td>
    <td>L</td>
  </tr>
  <tr>
    <td>3</td>
    <td>L</td>
    <td>H</td>
    <td>L</td>
    <td>H</td>
  </tr>
  <tr>
    <td>4</td>
    <td>L</td>
    <td>H</td>
    <td>H</td>
    <td>L</td>
  </tr>
  <tr>
    <td>1,2</td>
    <td>H</td>
    <td>L</td>
    <td>L</td>
    <td>L</td>
  </tr>
  <tr>
    <td>3,4</td>
    <td>L</td>
    <td>H</td>
    <td>L</td>
    <td>L</td>
  </tr>
  <tr>
    <td>1,3</td>
    <td>H</td>
    <td>H</td>
    <td>L</td>
    <td>H</td>
  </tr>
  <tr>
    <td>2,4</td>
    <td>H</td>
    <td>H</td>
    <td>H</td>
    <td>L</td>
  </tr>
  <tr>
    <td>1,4</td>
    <td colspan="4">switch 1/4</td>
  </tr>
  <tr>
    <td>2,3</td>
    <td colspan="4">switch 2/3</td>
  </tr>
  <tr>
    <td>1,2,3</td>
    <td colspan="4">switch 1,2/3</td>
  </tr>
  <tr>
    <td>2,3,4</td>
    <td colspan="4">switch 2,4/3</td>
  </tr>
  <tr>
    <td>1,3,4</td>
    <td colspan="4">switch 3,4/1</td>
  </tr>
  <tr>
    <td>1,2,4</td>
    <td colspan="4">switch 2,4/1</td>
  </tr>
  <tr>
    <td>1,2,3,4</td>
    <td>H</td>
    <td>H</td>
    <td>L</td>
    <td>L</td>
  </tr>
  <tr>
    <td>ниодного</td>
    <td>L</td>
    <td>L</td>
    <td>L</td>
    <td>L</td>
  </tr>
</table>
