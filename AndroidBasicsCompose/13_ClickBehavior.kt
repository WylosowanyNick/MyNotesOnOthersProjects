package com.example.lemonade

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.BorderStroke
import androidx.compose.foundation.Image
import androidx.compose.foundation.border
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material.Text
import androidx.compose.runtime.Composable

import androidx.compose.runtime.getValue // Te biblioteki trzeba ręcznie
import androidx.compose.runtime.setValue // zimportować, aby działały te dwie na dole
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember

import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import com.example.lemonade.ui.theme.LemonadeTheme

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            LemonadeTheme {
                LemonadeApp()
            }
        }
    }
}

@Composable
fun LemonadeMakingSteps(modifier: Modifier = Modifier) {
    var index by remember { mutableStateOf(1) }
    var numberOfTaps by remember { mutableStateOf(0) }
    val descriptionId: Int
    val description2Id: Int
    val imageId: Int

    when(index) {
        1 -> {
            descriptionId = R.string.lemonade_step1
            imageId = R.drawable.lemon_tree
            description2Id = R.string.lemon_tree
        }
        2 -> {
            descriptionId = R.string.lemonade_step2
            imageId = R.drawable.lemon_squeeze
            description2Id = R.string.lemon
        }
        3 -> {
            descriptionId = R.string.lemonade_step3
            imageId = R.drawable.lemon_drink
            description2Id = R.string.glass_of_lemonade
        }
        else -> {
            descriptionId = R.string.lemonade_step4
            imageId = R.drawable.lemon_restart
            description2Id = R.string.empty_glass
        }
    }

    Column(
        modifier = modifier,
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        Text(text = stringResource(id = descriptionId), fontSize = 18.sp)
        Spacer(modifier = Modifier.height(16.dp))
        Image(
            painter = painterResource(id = imageId),
            contentDescription = stringResource(id = description2Id),
            modifier = Modifier
                .wrapContentSize()
                .clickable {
                    if(index != 2) index++ // rekompozycja następuje dopiero po wyjściu ze scopu
                    else {
                        if(numberOfTaps == 0) numberOfTaps = (2..4).random()
                        else if(numberOfTaps == 1) index++
                        numberOfTaps--
                    }

                    if(index > 4) index = 1
                } // <- tu jest koniec scopu
                .border( // ładna obwódka
                    BorderStroke(2.dp, Color(105, 205, 216)),
                    shape = RoundedCornerShape(4.dp)
                )
        )
    }
}

@Preview(showBackground = true)
@Composable
fun LemonadeApp() {
    LemonadeTheme {
        LemonadeMakingSteps(
            Modifier
                .fillMaxSize()
                .wrapContentSize(Alignment.Center)
        )
    }
}