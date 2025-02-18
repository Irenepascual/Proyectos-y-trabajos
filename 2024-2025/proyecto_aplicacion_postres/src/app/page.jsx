import RecetasListSinFav from "/src/app/componentes/BuscadorListadoRecetasSinFav.js";
import HeaderSinSesion from "@/app/componentes/HeaderSinSesion.js";
import "/src/app/style-recetas.css";

/*
* Devuelve el home inicial (donde comienza la página
* web al abrirla), donde hay una cabecera para
* usuario no registrado y una lista de todos las 
* recetas aprobadas junto su información. 
*/
async function Home() {  
    return (
        <div>
            <HeaderSinSesion />
            <RecetasListSinFav />
        </div>
    );
}

export default Home;