import serial

def sendAnimToDisplay(display:serial,index):
    """
    case 0:
        wakeup();
        break;
    case 1:
        center_eyes(true);
        break;
    case 2:
        move_right_big_eye();
        break;
    case 3:
        move_left_big_eye();
        break;
    case 4:      
        blink(10);
        break;
    case 5:
        blink(20);
        break;
    case 6:
        happy_eye();      
        break;
    case 7:
        sleep();
        break;
    """
    cmd = 'A'+str(int(index))
    cmdByte = bytes(cmd, 'utf-8')
    print("writing ",cmd,cmdByte)
    display.write(cmdByte)
