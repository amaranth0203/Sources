<?php

    class News extends CI_Controller {

        public function __construct( ) {
            parent::__construct( ) ;
            $this->load->model( 'news_model' ) ;
            $this->load->helper( 'url_helper' ) ;
        }

        public function index( ) {
            $data[ 'news' ] = $this->news_model->get_news( ) ;
            $data[ 'title' ] = 'News archive' ;
            error_log( '[+] wassup in method index inside class News 2' ) ;

            $this->load->view( 'templates/header' , $data ) ;
            $this->load->view( 'news/index' , $data ) ;
            $this->load->view( 'templates/footer' ) ;
        }

        public function view( $slug = NULL ) {
            $data[ 'news_item' ] = $this->news_model->get_news( $slug ) ;
            error_log( print_r( $data[ 'news_item' ] ) ) ;
        }

    }

?>